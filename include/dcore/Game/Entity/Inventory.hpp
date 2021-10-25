#pragma once
#include <dcore/Game/Entity/Item.hpp>
#include <vector>

namespace dg::entity
{
	class Inventory
	{
	public:
		const std::vector<Item> &GetItems() const;
		const Item              &GetItem(int index) const;
		Item                    &GetItem(int index);

		class Iterator
		{
			Inventory &inv;
			int        index;

		public:
			Item     &operator*() const;
			Item     *operator->() const;
			Iterator &operator++();
			bool      operator==(const Iterator &other) const;
		};

		class ConstIterator
		{
			const Inventory &inv;
			int              index;

		public:
			const Item &operator*() const;
			const Item *operator->() const;
			Iterator   &operator++();
			bool        operator==(const ConstIterator &other) const;
		};

		Iterator      begin() const;
		Iterator      end() const;
		ConstIterator cbegin() const;
		ConstIterator cend() const;

	private:
		std::vector<Item> Items_;
	};
} // namespace dg::entity
