#include "pusha/notify.hpp"
#include "pusha.h"

#include <exception>

namespace pusha{

notify::notify(key const& ec_key, std::string_view const& subscriber)
	: sub_(subscriber), key_(ec_key){}

/*notify::notify(key&& ec_key, std::string_view const& subscriber)
	: sub_(subscriber), key_(std::move(ec_key))
{}*/

notify::notify(EC_KEY* key, std::string_view const& subscriber)
	: sub_(subscriber), key_(key)
{}

std::string_view notify::subscriber() const noexcept
{
	return sub_;
}

void notify::subscriber(std::string_view const& new_subscriber) noexcept
{
	sub_ = new_subscriber;
}

int notify::make(pusha_http_request& req,
				std::string_view const& endpoint,
				std::string_view const& p256dh,
				std::string_view const& auth,
				unsigned expiration,
				unsigned ttl,
				const void* payload /* = NULL */,
				std::size_t payload_len /* = 0 */,
				Pusha_HTTP_Version ver /* = pusha_HTTPver_1_1 */) noexcept
{
	return pusha_notify_http(&req,
			key_.get_key(),
			endpoint.data(), endpoint.size(),
			sub_.data(), sub_.size(),
			p256dh.data(), p256dh.size(),
			auth.data(), auth.size(),
			expiration, ttl,
			payload, payload_len, ver);
}

int notify::make(pusha_http_headers& headers,
		pusha_payload* pp,
		std::string_view const& endpoint,
		std::string_view const& p256dh,
		std::string_view const& auth,
		unsigned expiration,
		const void* payload /* = NULL */, std::size_t payload_len /* = 0 */) noexcept
{
	return pusha_notify(&headers, pp,
				key_.get_key(),
				endpoint.data(), endpoint.size(),
				sub_.data(), sub_.size(),
				p256dh.data(), p256dh.size(),
				auth.data(), auth.size(),
				expiration,
				payload, payload_len);
}

void notify::send_notification(std::string const& payload,
                               std::string_view const& endpoint,
                               std::string_view const& p256dh,
                               std::string_view const& auth,
                               unsigned int expiration)
{
    pusha_http_headers headers = {};
    pusha_payload pp = {};

    int err = this->make(headers, &pp, endpoint, p256dh, auth, expiration, payload.c_str(),
                          payload.size());
    throw_on_err(err);

    err = send_web_push(endpoint.data(), endpoint.size(), &headers, pp.cipher_payload,
                        pp.cipher_payload_len, 0);
    throw_on_err(err);

    free_pusha_http_headers(&headers);
    free_pusha_payload(&pp);
}

void
notify::create_and_send_notification(const key &key,
                                     const std::string_view &subscriber,
                                     const std::string &payload,
                                     const std::string_view &endpoint,
                                     const std::string_view &p256dh,
                                     const std::string_view &auth,
                                     unsigned int expiration)
{
    notify(key, subscriber).send_notification(payload, endpoint, p256dh, auth, expiration);
}

void notify::throw_on_err(int err)
{
    if (err) {
        std::error_code ec = make_error_code((pusha::errc) err);
        throw std::runtime_error(ec.message() + ", Error Code: " + std::to_string(err));
    }
}

}//pusha
