//: ----------------------------------------------------------------------------
//: Copyright (C) 2017 Verizon.  All Rights Reserved.
//: All Rights Reserved
//:
//: \file:    acl.h
//: \details: TODO
//: \author:  Devender Singh
//: \date:    07/14/2017
//:
//:   Licensed under the Apache License, Version 2.0 (the "License");
//:   you may not use this file except in compliance with the License.
//:   You may obtain a copy of the License at
//:
//:       http://www.apache.org/licenses/LICENSE-2.0
//:
//:   Unless required by applicable law or agreed to in writing, software
//:   distributed under the License is distributed on an "AS IS" BASIS,
//:   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//:   See the License for the specific language governing permissions and
//:   limitations under the License.
//:
//: ----------------------------------------------------------------------------
#ifndef _ACL_H_
#define _ACL_H_
//: ----------------------------------------------------------------------------
//: includes
//: ----------------------------------------------------------------------------
#include "waflz/def.h"
#include <strings.h>
#include <string>
#include <set>
//: ----------------------------------------------------------------------------
//: fwd decl's
//: ----------------------------------------------------------------------------
namespace waflz_pb {
        class profile;
        class profile_access_settings_t;
        class event;
        class acl;
}
namespace ns_waflz {
class regex;
class nms;
class geoip2_mmdb;
//: ----------------------------------------------------------------------------
//: types
//: ----------------------------------------------------------------------------
//: ----------------------------------------------------------------------------
//: acl
//: ----------------------------------------------------------------------------
class acl
{
public:
        // -------------------------------------------------
        // public methods
        // -------------------------------------------------
        acl(geoip2_mmdb &a_geoip2_mmdb);
        ~acl();
        int32_t compile();
        int32_t process(waflz_pb::event **ao_event, bool &ao_whitelist, void *a_ctx, rqst_ctx **ao_rqst_ctx = NULL);
        int32_t process_whitelist(bool &ao_match, rqst_ctx &a_ctx);
        int32_t process_blacklist(waflz_pb::event **ao_event, rqst_ctx &a_ctx);
        int32_t process_settings(waflz_pb::event **ao_event, rqst_ctx &a_ctx);
        //: ------------------------------------------------
        //:               G E T T E R S
        //: ------------------------------------------------
        //: ------------------------------------------------
        //: \details Get last error message string
        //: \return  last error message (in buffer)
        //: ------------------------------------------------
        const char *get_err_msg(void)
        {
                return m_err_msg;
        }
        waflz_pb::acl *get_pb(void) { return m_pb; }
private:
        // -------------------------------------------------
        // private types
        // -------------------------------------------------
        struct ci_less_comp
        {
                bool operator() (const std::string& lhs, const std::string& rhs) const
                {
                        return strcasecmp(lhs.c_str(), rhs.c_str()) < 0;
                }
        };
        // -------------------------------------------------
        // private types
        // -------------------------------------------------
        typedef std::set<uint32_t> asn_set_t;
        typedef std::set <std::string, ci_less_comp> stri_set_t;
        // -------------------------------------------------
        // private methods
        // -------------------------------------------------
        // disallow copy/assign
        acl(const acl &);
        acl& operator=(const acl &);
        // -------------------------------------------------
        // private members
        // -------------------------------------------------
        char m_err_msg[WAFLZ_ERR_LEN];
        waflz_pb::acl *m_pb;
        // -------------------------------------------------
        // *************************************************
        // geoip2 support
        // *************************************************
        // -------------------------------------------------
        geoip2_mmdb &m_geoip2_mmdb;
        // ip
        nms *m_ip_whitelist;
        nms *m_ip_blacklist;
        // country
        stri_set_t m_country_whitelist;
        stri_set_t m_country_blacklist;
        // asn
        asn_set_t m_asn_whitelist;
        asn_set_t m_asn_blacklist;
        // url
        regex *m_url_rx_whitelist;
        regex *m_url_rx_blacklist;
        // user-agent
        regex *m_ua_rx_whitelist;
        regex *m_ua_rx_blacklist;
        // referer
        regex *m_referer_rx_whitelist;
        regex *m_referer_rx_blacklist;
        // cookie
        regex *m_cookie_rx_whitelist;
        regex *m_cookie_rx_blacklist;
        // methods
        stri_set_t m_allowed_http_methods;
        // protocol versions
        stri_set_t m_allowed_http_versions;
        // content types
        stri_set_t m_allowed_request_content_types;
        // extensions
        stri_set_t m_disallowed_extensions;
        // headers
        stri_set_t m_disallowed_headers;
};
}
#endif
