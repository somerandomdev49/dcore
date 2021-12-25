#pragma once
#include <dg/Entity/Item.hpp>
#include <vector>

namespace dg::entity
{
	class Inventory
	{
	public:
		const std::vector<Item> &GetItems() const;
		const Item &GetItem(int index) const;
		Item &GetItem(int index);

		class Iterator
		{
			friend Inventory;
			Inventory *inv;
			int index;

			Iterator(Inventory *inv, int index) : inv(inv), index(index) {}

		public:
			Item &operator*() const { return inv->Items_[index]; }
			Item *operator->() const { return &inv->Items_[index]; }
			Iterator &operator++() { ++index; }
			bool operator==(const Iterator &other) const { return inv == other.inv && index == other.index; }
		};

		class ConstIterator
		{
			const Inventory &inv;
			int index;

		public:
			const Item &operator*() const;
			const Item *operator->() const;
			Iterator &operator++();
			bool operator==(const ConstIterator &other) const;
		};

		Iterator begin() { return Iterator(this, 0); }
		Iterator end() { return Iterator(this, Items_.size()); }
		ConstIterator cbegin() const;
		ConstIterator cend() const;

	private:
		friend class Iterator;
		friend class ConstIterator;
		std::vector<Item> Items_;
	};
} // namespace dg::entity
