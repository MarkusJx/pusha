#ifndef PUSHA_NOTIFY_HPP__
#define PUSHA_NOTIFY_HPP__

#include <cstdlib>
#include <string_view>

#include "pusha/http.h"
#include "pusha/ec_keys.hpp"
#include "pusha/error.hpp"

namespace pusha{

class notify{
	public:
		notify(key const&, std::string_view const& subscriber);
		notify(key&&, std::string_view const& subscriber);
		notify(EC_KEY* key, std::string_view const& subscriber);

		std::string_view subscriber() const noexcept;
		void subscriber(std::string_view const&) noexcept;

		int make(pusha_http_request& req,
				std::string_view const& endpoint,
				std::string_view const& p256dh,
				std::string_view const& auth,
				unsigned expiration, unsigned ttl,
				const void* payload = nullptr, std::size_t payload_len = 0,
				Pusha_HTTP_Version ver = pusha_HTTPver_1_1) noexcept;

		int make(pusha_http_headers& headers,
				pusha_payload* pp,
				std::string_view const& endpoint,
				std::string_view const& p256dh,
				std::string_view const& auth,
				unsigned expiration,
				const void* payload = nullptr, std::size_t payload_len = 0
				) noexcept;

        void send_notification(std::string const& payload,
                               std::string_view const& endpoint,
                               std::string_view const& p256dh,
                               std::string_view const& auth,
                               unsigned int expiration);

        static void create_and_send_notification(key const& key,
                                                 std::string_view const& subscriber,
                                                 std::string const& payload,
                                                 std::string_view const& endpoint,
                                                 std::string_view const& p256dh,
                                                 std::string_view const& auth,
                                                 unsigned int expiration);
	private:
		std::string_view 	sub_;
		key					key_;

        static void throw_on_err(int err);
};

}//pusha

#endif /* PUSHA_NOTIFY_HPP__ */
