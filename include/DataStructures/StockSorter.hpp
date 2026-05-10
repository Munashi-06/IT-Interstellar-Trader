#pragma once
#include "Entities/Item.hpp"
#include <utility>

namespace StockSorter {

    constexpr int QS_THRESHOLD = 40; // Threshold

    // --- 1. INSERTION SORT (For small partitions) ---
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

    // --- 2. MEDIAN SELECTION (Second Improvement) ---
    template<typename Iterator, typename Compare>
    inline Iterator medianOfThree(Iterator l, Iterator mid, Iterator r, Compare comp) {
        // Logic to find the median using the custom comparator
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

    // --- 3. PARTITION FOR QUICKSORT ---
    template<typename Iterator, typename Compare>
    inline Iterator customPartition(Iterator begin, Iterator end, Compare comp) {
        // Second improvement: Select the median among the first, middle, and last
        Iterator mid = begin + (end - begin) / 2;
        Iterator pivotIt = medianOfThree(begin, mid, end - 1, comp);
        
        // Move the pivot to the end using classic std::swap
        std::swap(*pivotIt, *(end - 1));
        auto pivot = *(end - 1); 

        Iterator i = begin;
        for (Iterator j = begin; j != end - 1; ++j) {
            if (comp(*j, pivot)) {
                std::swap(*i, *j); // Direct swap of values
                ++i;
            }
        }
        std::swap(*i, *(end - 1)); // Place the pivot in its final position
        return i;
    }

    // --- 4. HYBRID ALGORITHM (Third Improvement) ---
    template<typename Iterator, typename Compare>
    inline void hybridSort(Iterator begin, Iterator end, Compare comp) {
        auto size = std::distance(begin, end);

        // Third improvement: call insertion_sort when the size is less than QS_THRESHOLD[cite: 10, 11]
        if (size < QS_THRESHOLD) {
            insertionSort(begin, end, comp);
            return;
        }

        Iterator pivotIdx = customPartition(begin, end, comp);
        
        // Sort the smaller partition first (First improvement applied to recursion)[cite: 11]
        if (std::distance(begin, pivotIdx) < std::distance(pivotIdx + 1, end)) {
            hybridSort(begin, pivotIdx, comp);
            hybridSort(pivotIdx + 1, end, comp);
        } else {
            hybridSort(pivotIdx + 1, end, comp);
            hybridSort(begin, pivotIdx, comp);
        }
    }

    // ==========================================
    // YOUR GLOBAL SORTING FUNCTIONS
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
            
            // If they have the same quantity, tie-break by the item name
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