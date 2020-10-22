#pragma once

#include <utility>

namespace RAII {
	template<typename Provider>
	class Booking {
	public:
		Booking(Provider* ptr, int& ctr) : provider_(ptr), counter_(ctr) {}

		Booking(const Booking&) = delete;
		void operator=(const Booking&) = delete;

		Booking(Booking&& obj) : provider_(obj.provider_), counter_(obj.counter_) {
			obj.provider_ = nullptr;
		}

		void operator=(Booking&& obj) {
			counter_ = obj.counter_;
			provider_ = obj.provider_;
			obj.provider_ = nullptr;
		}

		~Booking() {
			if (provider_) {
				provider_->CancelOrComplete(*this);
			}
		}
	private:
		Provider* provider_;
		int& counter_;
	};
}