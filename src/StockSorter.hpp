#pragma once
#include "Item.hpp"
#include <utility>

namespace StockSorter {

    constexpr int QS_THRESHOLD = 40; // Umbral

    // --- 1. INSERTION SORT (Para particiones pequeñas) ---
    template<typename Iterator, typename Compare>
    inline void insertionSort(Iterator begin, Iterator end, Compare comp) {
        if (begin == end) return;
        for (Iterator i = begin + 1; i != end; ++i) {
            auto val = std::move(*i);
            Iterator j = i;
            while (j != begin && comp(val, *(j - 1))) {
                *j = std::move(*(j - 1));
                --j;
            }
            *j = std::move(val);
        }
    }

    // --- 2. SELECCIÓN DE MEDIANA (Segunda Mejora) ---
    template<typename Iterator, typename Compare>
    inline Iterator medianOfThree(Iterator l, Iterator mid, Iterator r, Compare comp) {
        // Lógica para encontrar la mediana usando el comparador personalizado
        if (comp(*l, *mid)) {
            if (comp(*mid, *r)) return mid;
            else if (comp(*l, *r)) return r;
            else return l;
        } else {
            if (comp(*l, *r)) return l;
            else if (comp(*mid, *r)) return r;
            else return mid;
        }
    }

    // --- 3. PARTICIÓN PARA QUICKSORT ---
    template<typename Iterator, typename Compare>
    inline Iterator customPartition(Iterator begin, Iterator end, Compare comp) {
        // Segunda mejora: Seleccionar la mediana entre el primero, el medio y el último
        Iterator mid = begin + (end - begin) / 2;
        Iterator pivotIt = medianOfThree(begin, mid, end - 1, comp);
        
        // Movemos el pivote al final usando std::swap clásico
        std::swap(*pivotIt, *(end - 1));
        auto pivot = *(end - 1); 

        Iterator i = begin;
        for (Iterator j = begin; j != end - 1; ++j) {
            if (comp(*j, pivot)) {
                std::swap(*i, *j); // Intercambio directo de valores
                ++i;
            }
        }
        std::swap(*i, *(end - 1)); // Colocamos el pivote en su posición final
        return i;
    }

    // --- 4. ALGORITMO HÍBRIDO (Tercera Mejora) ---
    template<typename Iterator, typename Compare>
    inline void hybridSort(Iterator begin, Iterator end, Compare comp) {
        auto size = std::distance(begin, end);

        // Tercera mejora: llamar a insertion_sort cuando el tamaño es menor a QS_THRESHOLD[cite: 10, 11]
        if (size < QS_THRESHOLD) {
            insertionSort(begin, end, comp);
            return;
        }

        Iterator pivotIdx = customPartition(begin, end, comp);
        
        // Ordenamos primero la partición más pequeña (Primera mejora aplicada a recursividad)[cite: 11]
        if (std::distance(begin, pivotIdx) < std::distance(pivotIdx + 1, end)) {
            hybridSort(begin, pivotIdx, comp);
            hybridSort(pivotIdx + 1, end, comp);
        } else {
            hybridSort(pivotIdx + 1, end, comp);
            hybridSort(begin, pivotIdx, comp);
        }
    }

    // ==========================================
    // TUS FUNCIONES GLOBALES DE ORDENAMIENTO
    // ==========================================

    inline void sortByName(std::vector<std::optional<ItemStack>>& stock, bool ascending, const std::unordered_map<std::string, std::unique_ptr<Item>>& catalog) {
        hybridSort(stock.begin(), stock.end(), [&](const std::optional<ItemStack>& a, const std::optional<ItemStack>& b) {
            if (!a && !b) return false;
            if (!a) return false;
            if (!b) return true;
            const auto& itemA = catalog.at(a->itemID);
            const auto& itemB = catalog.at(b->itemID);
            if (ascending) return itemA->getName() < itemB->getName();
            return itemA->getName() > itemB->getName();
        });
    }

    inline void sortByCategory(std::vector<std::optional<ItemStack>>& stock, bool ascending, const std::unordered_map<std::string, std::unique_ptr<Item>>& catalog) {
        hybridSort(stock.begin(), stock.end(), [&](const std::optional<ItemStack>& a, const std::optional<ItemStack>& b) {
            if (!a && !b) return false;
            if (!a) return false;
            if (!b) return true;
            const auto& itemA = catalog.at(a->itemID);
            const auto& itemB = catalog.at(b->itemID);
            if (itemA->getCategory() == itemB->getCategory()) return itemA->getName() < itemB->getName(); 
            if (ascending) return itemA->getCategory() < itemB->getCategory();
            return itemA->getCategory() > itemB->getCategory();
        });
    }

    inline void sortByQuality(std::vector<std::optional<ItemStack>>& stock, bool ascending, const std::unordered_map<std::string, std::unique_ptr<Item>>& catalog) {
        hybridSort(stock.begin(), stock.end(), [&](const std::optional<ItemStack>& a, const std::optional<ItemStack>& b) {
            if (!a && !b) return false;
            if (!a) return false;
            if (!b) return true;
            const auto& itemA = catalog.at(a->itemID);
            const auto& itemB = catalog.at(b->itemID);
            if (itemA->getRarity() == itemB->getRarity()) return itemA->getName() < itemB->getName();
            if (ascending) return itemA->getRarity() < itemB->getRarity();
            return itemA->getRarity() > itemB->getRarity();
        });
    }

    inline void sortByPrice(std::vector<std::optional<ItemStack>>& stock, bool ascending, const std::unordered_map<std::string, std::unique_ptr<Item>>& catalog) {
        hybridSort(stock.begin(), stock.end(), [&](const std::optional<ItemStack>& a, const std::optional<ItemStack>& b) {
            if (!a && !b) return false;
            if (!a) return false;
            if (!b) return true;
            const auto& itemA = catalog.at(a->itemID);
            const auto& itemB = catalog.at(b->itemID);
            if (itemA->getBasePrice() == itemB->getBasePrice()) return itemA->getName() < itemB->getName();
            if (ascending) return itemA->getBasePrice() < itemB->getBasePrice();
            return itemA->getBasePrice() > itemB->getBasePrice();
        });
    }

    inline void sortByQuantity(std::vector<std::optional<ItemStack>>& stock, bool ascending, const std::unordered_map<std::string, std::unique_ptr<Item>>& catalog) {
        hybridSort(stock.begin(), stock.end(), [&](const std::optional<ItemStack>& a, const std::optional<ItemStack>& b) {
            if (!a && !b) return false;
            if (!a) return false;
            if (!b) return true;
            
            // Si tienen la misma cantidad, desempatamos por el nombre del objeto
            if (a->quantity == b->quantity) {
                const auto& itemA = catalog.at(a->itemID);
                const auto& itemB = catalog.at(b->itemID);
                return itemA->getName() < itemB->getName();
            }
            
            if (ascending) return a->quantity < b->quantity;
            return a->quantity > b->quantity;
        });
    }
}