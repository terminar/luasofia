/* vim: set ts=8 et sw=4 sta ai cin: */
#include <lauxlib.h>
#include <lua.h>
#include <lualib.h>

#include "luasofia_tags.h"
#include "luasofia_utils.h"
#include "luasofia_struct.h"

#include <sofia-sip/sip.h>
#include <sofia-sip/sip_tag.h>

typedef struct lua_sip_s lua_sip_t;

#define SIP_MTABLE "lua_sip_t"

struct lua_sip_s {
    sip_t *sip;
    lua_State *L;
};

static const luasofia_struct_info_t url_info[] = {
{"pad",      luasofia_struct_get_char,   luasofia_struct_set_char,   offsetof(url_t, url_pad), offsetof(url_t, url_type)},
{"type",     luasofia_struct_get_char,   luasofia_struct_set_char,   offsetof(url_t, url_type),     0},
{"root",     luasofia_struct_get_char,   luasofia_struct_set_char,   offsetof(url_t, url_root),     0},
{"scheme",   luasofia_struct_get_string, luasofia_struct_set_string, offsetof(url_t, url_scheme),   0},
{"user",     luasofia_struct_get_string, luasofia_struct_set_string, offsetof(url_t, url_user),     0},
{"password", luasofia_struct_get_string, luasofia_struct_set_string, offsetof(url_t, url_password), 0},
{"host",     luasofia_struct_get_string, luasofia_struct_set_string, offsetof(url_t, url_host),     0},
{"port",     luasofia_struct_get_string, luasofia_struct_set_string, offsetof(url_t, url_port),     0},
{"path",     luasofia_struct_get_string, luasofia_struct_set_string, offsetof(url_t, url_path),     0},
{"params",   luasofia_struct_get_string, luasofia_struct_set_string, offsetof(url_t, url_params),   0},
{"headers",  luasofia_struct_get_string, luasofia_struct_set_string, offsetof(url_t, url_headers),  0},
{"fragment", luasofia_struct_get_string, luasofia_struct_set_string, offsetof(url_t, url_fragment), 0},
{NULL, NULL, 0 }
};

int luasofia_url_create(lua_State *L)
{
    /* Push struct info table at stack */
    luasofia_struct_create_info_table(L, url_info);    
    /* Create struct with info table */
    return luasofia_struct_create(L, sizeof(url_t));
}

static const luaL_Reg sip_meths[] = {
    {NULL, NULL}
};

static const luaL_Reg sip_lib[] = {
    {"create_url",  luasofia_url_create },
    {NULL, NULL}
};

static const luasofia_tag_reg_t sip_tags[] = {
    { "SIPTAG_ANY", siptag_any },
    { "SIPTAG_END", siptag_end },
    { "SIPTAG_SIP", siptag_sip },
    { "SIPTAG_SIP_REF", siptag_sip_ref },
    { "SIPTAG_HEADER", siptag_header },
    { "SIPTAG_HEADER_REF", siptag_header_ref },
    { "SIPTAG_HEADER_STR", siptag_header_str },
    { "SIPTAG_HEADER_STR_REF", siptag_header_str_ref },
    { "SIPTAG_REQUEST", siptag_request },
    { "SIPTAG_REQUEST_REF", siptag_request_ref },
    { "SIPTAG_REQUEST_STR", siptag_request_str },
    { "SIPTAG_REQUEST_STR_REF", siptag_request_str_ref },
    { "SIPTAG_STATUS", siptag_status },
    { "SIPTAG_STATUS_REF", siptag_status_ref },
    { "SIPTAG_STATUS_STR", siptag_status_str },
    { "SIPTAG_STATUS_STR_REF", siptag_status_str_ref },
    { "SIPTAG_VIA", siptag_via },
    { "SIPTAG_VIA_REF", siptag_via_ref },
    { "SIPTAG_VIA_STR", siptag_via_str },
    { "SIPTAG_VIA_STR_REF", siptag_via_str_ref },
    { "SIPTAG_ROUTE", siptag_route },
    { "SIPTAG_ROUTE_REF", siptag_route_ref },
    { "SIPTAG_ROUTE_STR", siptag_route_str },
    { "SIPTAG_ROUTE_STR_REF", siptag_route_str_ref },
    { "SIPTAG_RECORD_ROUTE", siptag_record_route },
    { "SIPTAG_RECORD_ROUTE_REF", siptag_record_route_ref },
    { "SIPTAG_RECORD_ROUTE_STR", siptag_record_route_str },
    { "SIPTAG_RECORD_ROUTE_STR_REF", siptag_record_route_str_ref },
    { "SIPTAG_MAX_FORWARDS", siptag_max_forwards },
    { "SIPTAG_MAX_FORWARDS_REF", siptag_max_forwards_ref },
    { "SIPTAG_MAX_FORWARDS_STR", siptag_max_forwards_str },
    { "SIPTAG_MAX_FORWARDS_STR_REF", siptag_max_forwards_str_ref },
    { "SIPTAG_PROXY_REQUIRE", siptag_proxy_require },
    { "SIPTAG_PROXY_REQUIRE_REF", siptag_proxy_require_ref },
    { "SIPTAG_PROXY_REQUIRE_STR", siptag_proxy_require_str },
    { "SIPTAG_PROXY_REQUIRE_STR_REF", siptag_proxy_require_str_ref },
    { "SIPTAG_FROM", siptag_from },
    { "SIPTAG_FROM_REF", siptag_from_ref },
    { "SIPTAG_FROM_STR", siptag_from_str },
    { "SIPTAG_FROM_STR_REF", siptag_from_str_ref },
    { "SIPTAG_TO", siptag_to },
    { "SIPTAG_TO_REF", siptag_to_ref },
    { "SIPTAG_TO_STR", siptag_to_str },
    { "SIPTAG_TO_STR_REF", siptag_to_str_ref },
    { "SIPTAG_CALL_ID", siptag_call_id },
    { "SIPTAG_CALL_ID_REF", siptag_call_id_ref },
    { "SIPTAG_CALL_ID_STR", siptag_call_id_str },
    { "SIPTAG_CALL_ID_STR_REF", siptag_call_id_str_ref },
    { "SIPTAG_CSEQ", siptag_cseq },
    { "SIPTAG_CSEQ_REF", siptag_cseq_ref },
    { "SIPTAG_CSEQ_STR", siptag_cseq_str },
    { "SIPTAG_CSEQ_STR_REF", siptag_cseq_str_ref },
    { "SIPTAG_CONTACT", siptag_contact },
    { "SIPTAG_CONTACT_REF", siptag_contact_ref },
    { "SIPTAG_CONTACT_STR", siptag_contact_str },
    { "SIPTAG_CONTACT_STR_REF", siptag_contact_str_ref },
    { "SIPTAG_RSEQ", siptag_rseq },
    { "SIPTAG_RSEQ_REF", siptag_rseq_ref },
    { "SIPTAG_RSEQ_STR", siptag_rseq_str },
    { "SIPTAG_RSEQ_STR_REF", siptag_rseq_str_ref },
    { "SIPTAG_RACK", siptag_rack },
    { "SIPTAG_RACK_REF", siptag_rack_ref },
    { "SIPTAG_RACK_STR", siptag_rack_str },
    { "SIPTAG_RACK_STR_REF", siptag_rack_str_ref },
    { "SIPTAG_REQUEST_DISPOSITION", siptag_request_disposition },
    { "SIPTAG_REQUEST_DISPOSITION_REF", siptag_request_disposition_ref },
    { "SIPTAG_REQUEST_DISPOSITION_STR", siptag_request_disposition_str },
    { "SIPTAG_REQUEST_DISPOSITION_STR_REF", siptag_request_disposition_str_ref },
    { "SIPTAG_ACCEPT_CONTACT", siptag_accept_contact },
    { "SIPTAG_ACCEPT_CONTACT_REF", siptag_accept_contact_ref },
    { "SIPTAG_ACCEPT_CONTACT_STR", siptag_accept_contact_str  },
    { "SIPTAG_ACCEPT_CONTACT_STR_REF", siptag_accept_contact_str_ref },
    { "SIPTAG_REJECT_CONTACT", siptag_reject_contact },
    { "SIPTAG_REJECT_CONTACT_REF", siptag_reject_contact_ref },
    { "SIPTAG_REJECT_CONTACT_STR", siptag_reject_contact_str },
    { "SIPTAG_REJECT_CONTACT_STR_REF", siptag_reject_contact_str_ref },
    { "SIPTAG_EXPIRES", siptag_expires },
    { "SIPTAG_EXPIRES_REF", siptag_expires_ref },
    { "SIPTAG_EXPIRES_STR", siptag_expires_str },
    { "SIPTAG_EXPIRES_STR_REF", siptag_expires_str_ref },
    { "SIPTAG_DATE", siptag_date },
    { "SIPTAG_DATE_REF", siptag_date_ref },
    { "SIPTAG_DATE_STR", siptag_date_str },
    { "SIPTAG_DATE_STR_REF", siptag_date_str_ref },
    { "SIPTAG_RETRY_AFTER", siptag_retry_after },
    { "SIPTAG_RETRY_AFTER_REF", siptag_retry_after_ref },
    { "SIPTAG_RETRY_AFTER_STR", siptag_retry_after_str },
    { "SIPTAG_RETRY_AFTER_STR_REF", siptag_retry_after_str_ref },
    { "SIPTAG_TIMESTAMP", siptag_timestamp },
    { "SIPTAG_TIMESTAMP_REF", siptag_timestamp_ref },
    { "SIPTAG_TIMESTAMP_STR", siptag_timestamp_str },
    { "SIPTAG_TIMESTAMP_STR_REF", siptag_timestamp_str_ref },
    { "SIPTAG_MIN_EXPIRES", siptag_min_expires },
    { "SIPTAG_MIN_EXPIRES_REF", siptag_min_expires_ref },
    { "SIPTAG_MIN_EXPIRES_STR", siptag_min_expires_str },
    { "SIPTAG_MIN_EXPIRES_STR_REF", siptag_min_expires_str_ref },
    { "SIPTAG_SUBJECT", siptag_subject },
    { "SIPTAG_SUBJECT_REF", siptag_subject_ref },
    { "SIPTAG_SUBJECT_STR", siptag_subject_str },
    { "SIPTAG_SUBJECT_STR_REF", siptag_subject_str_ref },
    { "SIPTAG_PRIORITY", siptag_priority_ref },
    { "SIPTAG_PRIORITY_REF", siptag_priority_ref },
    { "SIPTAG_PRIORITY_STR", siptag_priority_str },
    { "SIPTAG_PRIORITY_STR_REF", siptag_priority_str_ref },
    { "SIPTAG_CALL_INFO", siptag_call_info },
    { "SIPTAG_CALL_INFO_REF", siptag_call_info_ref },
    { "SIPTAG_CALL_INFO_STR", siptag_call_info_str },
    { "SIPTAG_CALL_INFO_STR_REF", siptag_call_info_str_ref },
    { "SIPTAG_ORGANIZATION", siptag_organization },
    { "SIPTAG_ORGANIZATION_REF", siptag_organization_ref },
    { "SIPTAG_ORGANIZATION_STR", siptag_organization_str },
    { "SIPTAG_ORGANIZATION_STR_REF", siptag_organization_str_ref },
    { "SIPTAG_SERVER", siptag_server },
    { "SIPTAG_SERVER_REF", siptag_server_ref },
    { "SIPTAG_SERVER_STR", siptag_server_str },
    { "SIPTAG_SERVER_STR_REF", siptag_server_str_ref },
    { "SIPTAG_USER_AGENT", siptag_user_agent },
    { "SIPTAG_USER_AGENT_REF", siptag_user_agent_ref },
    { "SIPTAG_USER_AGENT_STR", siptag_user_agent_str },
    { "SIPTAG_USER_AGENT_STR_REF", siptag_user_agent_str_ref },
    { "SIPTAG_IN_REPLY_TO", siptag_in_reply_to },
    { "SIPTAG_IN_REPLY_TO_REF", siptag_in_reply_to_ref },
    { "SIPTAG_IN_REPLY_TO_STR", siptag_in_reply_to_str },
    { "SIPTAG_IN_REPLY_TO_STR_REF", siptag_in_reply_to_str_ref },
    { "SIPTAG_ACCEPT", siptag_accept },
    { "SIPTAG_ACCEPT_REF", siptag_accept_ref },
    { "SIPTAG_ACCEPT_STR", siptag_accept_str },
    { "SIPTAG_ACCEPT_STR_REF", siptag_accept_str_ref },
    { "SIPTAG_ACCEPT_ENCODING", siptag_accept_encoding },
    { "SIPTAG_ACCEPT_ENCODING_REF", siptag_accept_encoding_ref },
    { "SIPTAG_ACCEPT_ENCODING_STR", siptag_accept_encoding_str },
    { "SIPTAG_ACCEPT_ENCODING_STR_REF", siptag_accept_encoding_str_ref },
    { "SIPTAG_ACCEPT_LANGUAGE", siptag_accept_language },
    { "SIPTAG_ACCEPT_LANGUAGE_REF", siptag_accept_language_ref },
    { "SIPTAG_ACCEPT_LANGUAGE_STR", siptag_accept_language_str },
    { "SIPTAG_ACCEPT_LANGUAGE_STR_REF", siptag_accept_language_str_ref },
    { "SIPTAG_ALLOW", siptag_allow },
    { "SIPTAG_ALLOW_REF", siptag_allow_ref },
    { "SIPTAG_ALLOW_STR", siptag_allow_str },
    { "SIPTAG_ALLOW_STR_REF", siptag_allow_str_ref },
    { "SIPTAG_REQUIRE", siptag_require },
    { "SIPTAG_REQUIRE_REF", siptag_require_ref },
    { "SIPTAG_REQUIRE_STR", siptag_require_str },
    { "SIPTAG_REQUIRE_STR_REF", siptag_require_str_ref },
    { "SIPTAG_SUPPORTED", siptag_supported },
    { "SIPTAG_SUPPORTED_REF", siptag_supported_ref },
    { "SIPTAG_SUPPORTED_STR", siptag_supported_str },
    { "SIPTAG_SUPPORTED_STR_REF", siptag_supported_str_ref },
    { "SIPTAG_UNSUPPORTED", siptag_unsupported },
    { "SIPTAG_UNSUPPORTED_REF", siptag_unsupported_ref },
    { "SIPTAG_UNSUPPORTED_STR", siptag_unsupported_str },
    { "SIPTAG_UNSUPPORTED_STR_REF", siptag_unsupported_str_ref },
    { "SIPTAG_EVENT", siptag_event },
    { "SIPTAG_EVENT_REF", siptag_event_ref },
    { "SIPTAG_EVENT_STR", siptag_event_str },
    { "SIPTAG_EVENT_STR_REF", siptag_event_str_ref },
    { "SIPTAG_ALLOW_EVENTS", siptag_allow_events },
    { "SIPTAG_ALLOW_EVENTS_REF", siptag_allow_events_ref },
    { "SIPTAG_ALLOW_EVENTS_STR", siptag_allow_events_str },
    { "SIPTAG_ALLOW_EVENTS_STR_REF", siptag_allow_events_str_ref },
    { "SIPTAG_SUBSCRIPTION_STATE", siptag_subscription_state },
    { "SIPTAG_SUBSCRIPTION_STATE_REF", siptag_subscription_state_ref },
    { "SIPTAG_SUBSCRIPTION_STATE_STR", siptag_subscription_state_str },
    { "SIPTAG_SUBSCRIPTION_STATE_STR_REF", siptag_subscription_state_str_ref },
    { "SIPTAG_PROXY_AUTHENTICATE", siptag_proxy_authenticate },
    { "SIPTAG_PROXY_AUTHENTICATE_REF", siptag_proxy_authenticate_ref },
    { "SIPTAG_PROXY_AUTHENTICATE_STR", siptag_proxy_authenticate_str },
    { "SIPTAG_PROXY_AUTHENTICATE_STR_REF", siptag_proxy_authenticate_str_ref },
    { "SIPTAG_PROXY_AUTHENTICATION_INFO", siptag_proxy_authentication_info },
    { "SIPTAG_PROXY_AUTHENTICATION_INFO_REF", siptag_proxy_authentication_info_ref },
    { "SIPTAG_PROXY_AUTHENTICATION_INFO_STR", siptag_proxy_authentication_info_str },
    { "SIPTAG_PROXY_AUTHENTICATION_INFO_STR_REF", siptag_proxy_authentication_info_str_ref },
    { "SIPTAG_PROXY_AUTHORIZATION", siptag_proxy_authorization },
    { "SIPTAG_PROXY_AUTHORIZATION_REF", siptag_proxy_authorization_ref },
    { "SIPTAG_PROXY_AUTHORIZATION_STR", siptag_proxy_authorization_str },
    { "SIPTAG_PROXY_AUTHORIZATION_STR_REF", siptag_proxy_authorization_str_ref },
    { "SIPTAG_AUTHORIZATION", siptag_authorization },
    { "SIPTAG_AUTHORIZATION_REF", siptag_authorization_ref },
    { "SIPTAG_AUTHORIZATION_STR", siptag_authorization_str },
    { "SIPTAG_AUTHORIZATION_STR_REF", siptag_authorization_str_ref },
    { "SIPTAG_WWW_AUTHENTICATE", siptag_www_authenticate },
    { "SIPTAG_WWW_AUTHENTICATE_REF", siptag_www_authenticate_ref },
    { "SIPTAG_WWW_AUTHENTICATE_STR", siptag_www_authenticate_str },
    { "SIPTAG_WWW_AUTHENTICATE_STR_REF", siptag_www_authenticate_str_ref },
    { "SIPTAG_AUTHENTICATION_INFO", siptag_authentication_info },
    { "SIPTAG_AUTHENTICATION_INFO_REF", siptag_authentication_info_ref },
    { "SIPTAG_AUTHENTICATION_INFO_STR", siptag_authentication_info_str },
    { "SIPTAG_AUTHENTICATION_INFO_STR_REF", siptag_authentication_info_str_ref },
    { "SIPTAG_ERROR_INFO", siptag_error_info },
    { "SIPTAG_ERROR_INFO_REF", siptag_error_info_ref },
    { "SIPTAG_ERROR_INFO_STR", siptag_error_info_str },
    { "SIPTAG_ERROR_INFO_STR_REF", siptag_error_info_str_ref },
    { "SIPTAG_WARNING", siptag_warning },
    { "SIPTAG_WARNING_REF", siptag_warning_ref },
    { "SIPTAG_WARNING_STR", siptag_warning_str },
    { "SIPTAG_WARNING_STR_REF", siptag_warning_str_ref },
    { "SIPTAG_REFER_TO", siptag_refer_to },
    { "SIPTAG_REFER_TO_REF", siptag_refer_to_ref },
    { "SIPTAG_REFER_TO_STR", siptag_refer_to_str },
    { "SIPTAG_REFER_TO_STR_REF", siptag_refer_to_str_ref },
    { "SIPTAG_REFERRED_BY", siptag_referred_by },
    { "SIPTAG_REFERRED_BY_REF", siptag_referred_by_ref },
    { "SIPTAG_REFERRED_BY_STR", siptag_referred_by_str },
    { "SIPTAG_REFERRED_BY_STR_REF", siptag_referred_by_str_ref },
    { "SIPTAG_REPLACES", siptag_replaces },
    { "SIPTAG_REPLACES_REF", siptag_replaces_ref },
    { "SIPTAG_REPLACES_STR", siptag_replaces_str },
    { "SIPTAG_REPLACES_STR_REF", siptag_replaces_str_ref },
    { "SIPTAG_SESSION_EXPIRES", siptag_session_expires },
    { "SIPTAG_SESSION_EXPIRES_REF", siptag_session_expires_ref },
    { "SIPTAG_SESSION_EXPIRES_STR", siptag_session_expires_str },
    { "SIPTAG_SESSION_EXPIRES_STR_REF", siptag_session_expires_str_ref },
    { "SIPTAG_MIN_SE", siptag_min_se },
    { "SIPTAG_MIN_SE_REF", siptag_min_se_ref },
    { "SIPTAG_MIN_SE_STR", siptag_min_se_str },
    { "SIPTAG_MIN_SE_STR_REF", siptag_min_se_str_ref },
    { "SIPTAG_PATH", siptag_path },
    { "SIPTAG_PATH_REF", siptag_path_ref },
    { "SIPTAG_PATH_STR", siptag_path_str },
    { "SIPTAG_PATH_STR_REF", siptag_path_str_ref },
    { "SIPTAG_SERVICE_ROUTE", siptag_service_route },
    { "SIPTAG_SERVICE_ROUTE_REF", siptag_service_route_ref },
    { "SIPTAG_SERVICE_ROUTE_STR", siptag_service_route_str },
    { "SIPTAG_SERVICE_ROUTE_STR_REF", siptag_service_route_str_ref },
    { "SIPTAG_REASON", siptag_reason },
    { "SIPTAG_REASON_REF", siptag_reason_ref },
    { "SIPTAG_REASON_STR", siptag_reason_str },
    { "SIPTAG_REASON_STR_REF", siptag_reason_str_ref },
    { "SIPTAG_SECURITY_CLIENT", siptag_security_client },
    { "SIPTAG_SECURITY_CLIENT_REF", siptag_security_client_ref },
    { "SIPTAG_SECURITY_CLIENT_STR", siptag_security_client_str },
    { "SIPTAG_SECURITY_CLIENT_STR_REF", siptag_security_client_str_ref },
    { "SIPTAG_SECURITY_SERVER", siptag_security_server },
    { "SIPTAG_SECURITY_SERVER_REF", siptag_security_server_ref },
    { "SIPTAG_SECURITY_SERVER_STR", siptag_security_server_str },
    { "SIPTAG_SECURITY_SERVER_STR_REF", siptag_security_server_str_ref },
    { "SIPTAG_SECURITY_VERIFY", siptag_security_verify },
    { "SIPTAG_SECURITY_VERIFY_REF", siptag_security_verify_ref },
    { "SIPTAG_SECURITY_VERIFY_STR", siptag_security_verify_str },
    { "SIPTAG_SECURITY_VERIFY_STR_REF", siptag_security_verify_str_ref },
    { "SIPTAG_PRIVACY", siptag_privacy },
    { "SIPTAG_PRIVACY_REF", siptag_privacy_ref },
    { "SIPTAG_PRIVACY_STR", siptag_privacy_str },
    { "SIPTAG_PRIVACY_STR_REF", siptag_privacy_str_ref },
    { "SIPTAG_ETAG", siptag_etag },
    { "SIPTAG_ETAG_REF", siptag_etag_ref },
    { "SIPTAG_ETAG_STR", siptag_etag_str },
    { "SIPTAG_ETAG_STR_REF", siptag_etag_str_ref },
    { "SIPTAG_IF_MATCH", siptag_if_match },
    { "SIPTAG_IF_MATCH_REF", siptag_if_match_ref },
    { "SIPTAG_IF_MATCH_STR", siptag_if_match_str },
    { "SIPTAG_IF_MATCH_STR_REF", siptag_if_match_str_ref },
    { "SIPTAG_MIME_VERSION", siptag_mime_version },
    { "SIPTAG_MIME_VERSION_REF", siptag_mime_version_ref },
    { "SIPTAG_MIME_VERSION_STR", siptag_mime_version_str },
    { "SIPTAG_MIME_VERSION_STR_REF", siptag_mime_version_str_ref },
    { "SIPTAG_CONTENT_TYPE", siptag_content_type },
    { "SIPTAG_CONTENT_TYPE_REF", siptag_content_type_ref },
    { "SIPTAG_CONTENT_TYPE_STR", siptag_content_type_str },
    { "SIPTAG_CONTENT_TYPE_STR_REF", siptag_content_type_str_ref },
    { "SIPTAG_CONTENT_ENCODING", siptag_content_encoding },
    { "SIPTAG_CONTENT_ENCODING_REF", siptag_content_encoding_ref },
    { "SIPTAG_CONTENT_ENCODING_STR", siptag_content_encoding_str },
    { "SIPTAG_CONTENT_ENCODING_STR_REF", siptag_content_encoding_str_ref },
    { "SIPTAG_CONTENT_LANGUAGE", siptag_content_language },
    { "SIPTAG_CONTENT_LANGUAGE_REF", siptag_content_language_ref },
    { "SIPTAG_CONTENT_LANGUAGE_STR", siptag_content_language_str },
    { "SIPTAG_CONTENT_LANGUAGE_STR_REF", siptag_content_language_str_ref },
    { "SIPTAG_CONTENT_DISPOSITION", siptag_content_disposition },
    { "SIPTAG_CONTENT_DISPOSITION_REF", siptag_content_disposition_ref },
    { "SIPTAG_CONTENT_DISPOSITION_STR", siptag_content_disposition_str },
    { "SIPTAG_CONTENT_DISPOSITION_STR_REF", siptag_content_disposition_str_ref },
    { "SIPTAG_CONTENT_LENGTH", siptag_content_length },
    { "SIPTAG_CONTENT_LENGTH_REF", siptag_content_length_ref },
    { "SIPTAG_CONTENT_LENGTH_STR", siptag_content_length_str },
    { "SIPTAG_CONTENT_LENGTH_STR_REF", siptag_content_length_str_ref },
    { "SIPTAG_UNKNOWN", siptag_unknown },
    { "SIPTAG_UNKNOWN_REF", siptag_unknown_ref },
    { "SIPTAG_UNKNOWN_STR", siptag_unknown_str },
    { "SIPTAG_UNKNOWN_STR_REF", siptag_unknown_str_ref },
    { "SIPTAG_ERROR", siptag_error },
    { "SIPTAG_ERROR_REF", siptag_error_ref },
    { "SIPTAG_ERROR_STR", siptag_error_str },
    { "SIPTAG_ERROR_STR_REF", siptag_error_str_ref },
    { "SIPTAG_SEPARATOR", siptag_separator },
    { "SIPTAG_SEPARATOR_REF", siptag_separator_ref },
    { "SIPTAG_SEPARATOR_STR", siptag_separator_str },
    { "SIPTAG_SEPARATOR_STR_REF", siptag_separator_str_ref },
    { "SIPTAG_PAYLOAD", siptag_payload },
    { "SIPTAG_PAYLOAD_REF", siptag_payload_ref },
    { "SIPTAG_PAYLOAD_STR", siptag_payload_str },
    { "SIPTAG_PAYLOAD_STR_REF", siptag_payload_str_ref },
    { NULL, NULL}
};

static const luasofia_reg_const_t sip_constants[] = {
    {NULL, 0 }
};

int luaopen_luasofia_sip(lua_State *L)
{
    luaL_newmetatable(L, SIP_MTABLE);
    /* metatable.__index = metatable */
    lua_pushvalue(L, -1);
    lua_setfield(L, -2, "__index");
    luaL_register(L, NULL, sip_meths);
    lua_pop(L, 1);

    luasofia_tags_register(L, sip_tags);

    lua_getglobal(L, "luasofia");
    if(lua_isnil(L, -1)) {
        lua_newtable(L);
        lua_pushvalue(L, -1);
        lua_setglobal(L, "luasofia");
    }
    /* luasofia[sip] = table */
    lua_newtable(L);
    lua_pushvalue(L, -1);
    lua_setfield(L, -3, "sip");
    luaL_register(L, NULL, sip_lib);

    luasofia_register_constants(L, sip_constants);

    return 1;
}

