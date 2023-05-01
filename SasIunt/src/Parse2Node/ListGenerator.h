#pragma once

#define MAX_LEVEL 100;


namespace Sas {
	class ListGenerator
	{
	public:
		ListGenerator()
			:level({ 0 })
			, level_iterator(level.begin())
			, changed(true)
		{
			(*level_iterator) = 0;
			GetLevelInString();
		};
		

		void operator >>(uint16_t t) { level_iterator += t; *level_iterator = 0; changed = true; };
		void operator <<(uint16_t t) { level_iterator -= t; changed = true; };

		void operator++() { (*level_iterator)++;  changed = true; };
		void operator++(int) { (*level_iterator)++; changed = true; };

		void operator--() { (*level_iterator)--; changed = true; };
		void operator--(int) { (*level_iterator)--; changed = true;  };

	public:
		const std::string_view GetLevelInString() const
		{
			if (changed) {
				std::stringstream ss;
				for (auto level_ret : level)
					if (level_ret != 0)
						ss << level_ret << ".";
					else
						break;
				name_ret = ss.str();
				changed = false;
			}
			return name_ret;
		}

	private:
		std::array<uint16_t, 100>::iterator level_iterator;
		std::array<uint16_t, 100> level;
		mutable std::string name_ret;
		mutable bool changed = false;
	};
}
