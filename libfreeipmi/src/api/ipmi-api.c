/* 
   Copyright (C) 2003-2008 FreeIPMI Core Team

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software Foundation,
   Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA.  

*/

#if HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#ifdef STDC_HEADERS
#include <string.h>
#endif /* STDC_HEADERS */
#if HAVE_UNISTD_H
#include <unistd.h>
#endif /* HAVE_UNISTD_H */
#if TIME_WITH_SYS_TIME
#include <sys/time.h>
#include <time.h>
#else /* !TIME_WITH_SYS_TIME */
#if HAVE_SYS_TIME_H
#include <sys/time.h>
#else /* !HAVE_SYS_TIME_H */
#include <time.h>
#endif /* !HAVE_SYS_TIME_H */
#endif  /* !TIME_WITH_SYS_TIME */
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <assert.h>
#include <errno.h>

#include "freeipmi/api/ipmi-api.h"
#include "freeipmi/api/ipmi-messaging-support-cmds-api.h"
#include "freeipmi/cmds/ipmi-messaging-support-cmds.h"
#include "freeipmi/debug/ipmi-debug.h"
#include "freeipmi/driver/ipmi-kcs-driver.h"
#include "freeipmi/driver/ipmi-openipmi-driver.h"
#include "freeipmi/driver/ipmi-ssif-driver.h"
#include "freeipmi/interface/ipmi-kcs-interface.h"
#include "freeipmi/interface/ipmi-lan-interface.h"
#include "freeipmi/interface/ipmi-rmcpplus-interface.h"
#include "freeipmi/interface/rmcp-interface.h"
#include "freeipmi/locate/ipmi-locate.h"
#include "freeipmi/spec/ipmi-authentication-type-spec.h"
#include "freeipmi/spec/ipmi-privilege-level-spec.h"
#include "freeipmi/util/ipmi-cipher-suite-util.h"
#include "freeipmi/util/ipmi-util.h"

#include "ipmi-ctx.h"
#include "ipmi-err-wrappers-api.h"
#include "ipmi-fiid-wrappers-api.h"
#include "ipmi-lan-interface-api.h"
#include "ipmi-lan-session-common.h"
#include "ipmi-kcs-driver-api.h"
#include "ipmi-openipmi-driver-api.h"
#include "ipmi-ssif-driver-api.h"

#include "libcommon/ipmi-crypt.h"

#include "freeipmi-portability.h"
#include "debug-common.h"
#include "secure.h"

#define IPMI_SESSION_TIMEOUT         20000
#define IPMI_RETRANSMISSION_TIMEOUT  1000
#define IPMI_POLL_INTERVAL_USECS     60

#define GETHOSTBYNAME_AUX_BUFLEN     1024
extern int h_errno;

static char *ipmi_errmsg[] =
  {
    "success",			                                      /* 0 */
    "device null", 	                                              /* 1 */
    "device invalid",		                                      /* 2 */
    "permission denied",	                                      /* 3 */
    "username invalid",		                                      /* 4 */
    "password invalid",		                                      /* 5 */
    "k_g invalid",                                                    /* 6 */
    "privilege level insufficient",                                   /* 7 */
    "privilege level cannot be obtained for this user",               /* 8 */
    "authentication type unavailable for attempted privilege level",  /* 9 */
    "cipher suite id unavailable",                                    /* 10 */
    "password verification timeout",                                  /* 11 */
    "ipmi 2.0 unavailable",                                           /* 12 */
    "session timeout",		                                      /* 13 */
    "device already open",                                            /* 14 */
    "device not open",		                                      /* 15 */
    "device not supported",	                                      /* 16 */
    "device not found",                                               /* 17 */
    "driver timeout",                                                 /* 18 */
    "bad completion code: node busy/out of resources",                /* 19 */
    "bad completion code: command invalid/not supported",             /* 20 */
    "bad completion code: request data/parameter invalid", 	      /* 21 */
    "bad completion code",	                                      /* 22 */
    "bad rmcpplus status code",                                       /* 23 */
    "BMC busy",			                                      /* 24 */
    "out of memory",		                                      /* 25 */
    "invalid hostname",                                               /* 26 */
    "invalid parameters",	                                      /* 27 */
    "driver path required",                                           /* 28 */
    "internal IPMI error",	                                      /* 29 */
    "internal system error",	                                      /* 30 */
    "internal library error",	                                      /* 31 */
    "internal error",		                                      /* 32 */
    "errnum out of range",	                                      /* 33 */
  };

static void
_ipmi_ctx_init(struct ipmi_ctx *dev)
{
  assert(dev);

  memset(dev, '\0', sizeof(struct ipmi_ctx));
  dev->magic = IPMI_CTX_MAGIC;
  dev->type = IPMI_DEVICE_UNKNOWN;
}

ipmi_ctx_t
ipmi_ctx_create(void)
{
  struct ipmi_ctx *dev;

  if (!(dev = (struct ipmi_ctx *)malloc(sizeof(struct ipmi_ctx))))
    return NULL;

  _ipmi_ctx_init(dev);
  dev->errnum = IPMI_ERR_SUCCESS;

  return dev;
}

char *
ipmi_ctx_strerror(int errnum)
{
  if (errnum >= IPMI_ERR_SUCCESS && errnum <= IPMI_ERR_ERRNUMRANGE)
    return ipmi_errmsg[errnum];
  else
    return ipmi_errmsg[IPMI_ERR_ERRNUMRANGE];
}

int
ipmi_ctx_errnum(ipmi_ctx_t ctx)
{
  if (!ctx)
    return (IPMI_ERR_CTX_NULL);
  else if (ctx->magic != IPMI_CTX_MAGIC)
    return (IPMI_ERR_CTX_INVALID);
  else
    return (ctx->errnum);
}

static void 
_ipmi_outofband_free (ipmi_ctx_t ctx)
{
  /* Function Note: No need to set errnum - just return */
  assert(ctx && ctx->magic == IPMI_CTX_MAGIC);

  API_FIID_OBJ_DESTROY (ctx->io.outofband.rq.obj_rmcp_hdr);
  API_FIID_OBJ_DESTROY (ctx->io.outofband.rq.obj_lan_session_hdr);
  API_FIID_OBJ_DESTROY (ctx->io.outofband.rq.obj_rmcpplus_session_hdr);
  API_FIID_OBJ_DESTROY (ctx->io.outofband.rq.obj_lan_msg_hdr);
  API_FIID_OBJ_DESTROY (ctx->io.outofband.rq.obj_rmcpplus_session_trlr);

  API_FIID_OBJ_DESTROY (ctx->io.outofband.rs.obj_rmcp_hdr);
  API_FIID_OBJ_DESTROY (ctx->io.outofband.rs.obj_lan_session_hdr);
  API_FIID_OBJ_DESTROY (ctx->io.outofband.rs.obj_rmcpplus_session_hdr);
  API_FIID_OBJ_DESTROY (ctx->io.outofband.rs.obj_lan_msg_hdr);
  API_FIID_OBJ_DESTROY (ctx->io.outofband.rs.obj_rmcpplus_payload);
  API_FIID_OBJ_DESTROY (ctx->io.outofband.rs.obj_lan_msg_trlr);
  API_FIID_OBJ_DESTROY (ctx->io.outofband.rs.obj_rmcpplus_session_trlr);
}

static void 
_ipmi_inband_free (ipmi_ctx_t ctx)
{
  /* Function Note: No need to set errnum - just return */
  assert(ctx && ctx->magic == IPMI_CTX_MAGIC);
  
  if (ctx->type == IPMI_DEVICE_KCS && ctx->io.inband.kcs_ctx)
    ipmi_kcs_ctx_destroy(ctx->io.inband.kcs_ctx);
  if (ctx->type == IPMI_DEVICE_SSIF && ctx->io.inband.ssif_ctx)
    ipmi_ssif_ctx_destroy(ctx->io.inband.ssif_ctx);
  if (ctx->type == IPMI_DEVICE_OPENIPMI && ctx->io.inband.openipmi_ctx)
    ipmi_openipmi_ctx_destroy(ctx->io.inband.openipmi_ctx);

  API_FIID_OBJ_DESTROY (ctx->io.inband.rq.obj_hdr);
  API_FIID_OBJ_DESTROY (ctx->io.inband.rs.obj_hdr);
}

int
ipmi_ctx_open_outofband (ipmi_ctx_t ctx,
                         const char *hostname,
                         const char *username, 
                         const char *password, 
                         uint8_t authentication_type, 
                         uint8_t privilege_level,
                         unsigned int session_timeout,
                         unsigned int retransmission_timeout,
                         uint32_t workaround_flags,
                         uint32_t flags)
{
  struct sockaddr_in addr;
#ifdef HAVE_FUNC_GETHOSTBYNAME_R_6
  struct hostent hent;
  int h_errnop;
  char buf[GETHOSTBYNAME_AUX_BUFLEN];
#endif /* HAVE_FUNC_GETHOSTBYNAME_R_6 */
  struct hostent *hptr;
  uint32_t flags_mask = (IPMI_WORKAROUND_FLAGS_ACCEPT_SESSION_ID_ZERO
                         | IPMI_WORKAROUND_FLAGS_FORCE_PERMSG_AUTHENTICATION
                         | IPMI_WORKAROUND_FLAGS_CHECK_UNEXPECTED_AUTHCODE
                         | IPMI_WORKAROUND_FLAGS_BIG_ENDIAN_SEQUENCE_NUMBER
                         | IPMI_WORKAROUND_FLAGS_AUTHENTICATION_CAPABILITIES);
  
  API_ERR_CTX_CHECK (ctx && ctx->magic == IPMI_CTX_MAGIC);

  API_ERR_DEVICE_ALREADY_OPEN(ctx->type == IPMI_DEVICE_UNKNOWN);

  API_ERR_PARAMETERS(hostname
                     && !(username && strlen (username) > IPMI_MAX_USER_NAME_LENGTH)
                     && !(password && strlen (password) > IPMI_1_5_MAX_PASSWORD_LENGTH)
                     && IPMI_1_5_AUTHENTICATION_TYPE_VALID (authentication_type)
                     && IPMI_PRIVILEGE_LEVEL_VALID (privilege_level));

  API_ERR_PARAMETERS(!(strlen(hostname) > MAXHOSTNAMELEN));
   
  API_ERR_PARAMETERS(!(workaround_flags & ~flags_mask));

  ctx->type = IPMI_DEVICE_LAN;
  ctx->workaround_flags = workaround_flags;
  ctx->flags = flags;

#ifdef HAVE_FUNC_GETHOSTBYNAME_R_6
  memset(&hent, '\0', sizeof(struct hostent));
  API_ERR_HOSTNAME_INVALID_CLEANUP(!gethostbyname_r(hostname,
                                                    &hent,
                                                    buf,
                                                    GETHOSTBYNAME_AUX_BUFLEN,
                                                    &hptr,
                                                    &h_errnop));
  API_ERR_HOSTNAME_INVALID_CLEANUP(hptr);
#else  /* !HAVE_FUNC_GETHOSTBYNAME_R */
#error Additional threadsafe gethostbyname support needed
#endif /* !HAVE_FUNC_GETHOSTBYNAME_R */

  strncpy(ctx->io.outofband.hostname,
          hostname,
          MAXHOSTNAMELEN);

  ctx->io.outofband.remote_host.sin_family = AF_INET;
  ctx->io.outofband.remote_host.sin_port = htons(RMCP_AUX_BUS_SHUNT);
  ctx->io.outofband.remote_host.sin_addr = *(struct in_addr *) hptr->h_addr;
  
  memset(ctx->io.outofband.username, '\0', IPMI_MAX_USER_NAME_LENGTH+1);
  if (username)
    memcpy (ctx->io.outofband.username, 
	    username, 
	    strlen (username));
  memset(ctx->io.outofband.password, '\0', IPMI_1_5_MAX_PASSWORD_LENGTH+1);
  if (password)
    memcpy (ctx->io.outofband.password, 
	    password, 
	    strlen (password));
  ctx->io.outofband.privilege_level = privilege_level;
  ctx->io.outofband.session_timeout = (session_timeout ? session_timeout : IPMI_SESSION_TIMEOUT);
  ctx->io.outofband.retransmission_timeout = (retransmission_timeout ? retransmission_timeout : IPMI_RETRANSMISSION_TIMEOUT);

  API_ERR_PARAMETERS(ctx->io.outofband.retransmission_timeout < ctx->io.outofband.session_timeout);

  memset(&ctx->io.outofband.last_send, '\0', sizeof(struct timeval));
  memset(&ctx->io.outofband.last_received, '\0', sizeof(struct timeval));

  ctx->io.outofband.highest_received_sequence_number = 0;
  ctx->io.outofband.previously_received_list = 0xFF;
 
  ctx->io.outofband.authentication_type = authentication_type;

  API_FIID_OBJ_CREATE_CLEANUP (ctx->io.outofband.rq.obj_rmcp_hdr, tmpl_rmcp_hdr);
  API_FIID_OBJ_CREATE_CLEANUP (ctx->io.outofband.rq.obj_lan_session_hdr, tmpl_lan_session_hdr);
  API_FIID_OBJ_CREATE_CLEANUP (ctx->io.outofband.rq.obj_lan_msg_hdr, tmpl_lan_msg_hdr_rq);
  
  API_FIID_OBJ_CREATE_CLEANUP (ctx->io.outofband.rs.obj_rmcp_hdr, tmpl_rmcp_hdr);
  API_FIID_OBJ_CREATE_CLEANUP (ctx->io.outofband.rs.obj_lan_session_hdr, tmpl_lan_session_hdr);
  API_FIID_OBJ_CREATE_CLEANUP (ctx->io.outofband.rs.obj_lan_msg_hdr, tmpl_lan_msg_hdr_rs);
  API_FIID_OBJ_CREATE_CLEANUP (ctx->io.outofband.rs.obj_lan_msg_trlr, tmpl_lan_msg_trlr);
  
  /* Open client (local) UDP socket */
  /* achu: ephemeral ports are > 1023, so no way we will bind to an IPMI port */
  
  API_ERR_SYSTEM_ERROR_CLEANUP (!((ctx->io.outofband.sockfd = socket (AF_INET, SOCK_DGRAM, 0)) < 0));

  memset (&addr, 0, sizeof (struct sockaddr_in));
  addr.sin_family = AF_INET;
  addr.sin_port   = htons (0);
  addr.sin_addr.s_addr = htonl (INADDR_ANY);

  API_ERR_SYSTEM_ERROR_CLEANUP (!(bind(ctx->io.outofband.sockfd, 
                                       (struct sockaddr *)&addr,
                                       sizeof(struct sockaddr_in)) < 0));
  
  /* errnum set in ipmi_lan_open_session */
  if (ipmi_lan_open_session (ctx) < 0)
    goto cleanup;

  ctx->errnum = IPMI_ERR_SUCCESS;
  return (0);

 cleanup:
  if (ctx->io.outofband.sockfd)
    close (ctx->io.outofband.sockfd);
  _ipmi_outofband_free (ctx);
  ctx->type = IPMI_DEVICE_UNKNOWN;
  return (-1);
}

int 
ipmi_ctx_open_outofband_2_0 (ipmi_ctx_t ctx,
                             const char *hostname,
                             const char *username, 
                             const char *password, 
                             const unsigned char *k_g,
                             unsigned int k_g_len,
                             uint8_t privilege_level,
                             uint8_t cipher_suite_id,
                             unsigned int session_timeout,
                             unsigned int retransmission_timeout, 
                             uint32_t workaround_flags,
                             uint32_t flags)
{
  struct sockaddr_in addr;
#ifdef HAVE_FUNC_GETHOSTBYNAME_R_6
  struct hostent hent;
  int h_errnop;
  char buf[GETHOSTBYNAME_AUX_BUFLEN];
#endif /* HAVE_FUNC_GETHOSTBYNAME_R_6 */
  struct hostent *hptr;
  uint32_t flags_mask = (IPMI_WORKAROUND_FLAGS_AUTHENTICATION_CAPABILITIES
                         | IPMI_WORKAROUND_FLAGS_IGNORE_SOL_PAYLOAD_SIZE
                         | IPMI_WORKAROUND_FLAGS_IGNORE_SOL_PORT
                         | IPMI_WORKAROUND_FLAGS_INTEL_2_0_SESSION
                         | IPMI_WORKAROUND_FLAGS_SUPERMICRO_2_0_SESSION
                         | IPMI_WORKAROUND_FLAGS_SUN_2_0_SESSION);
  
  API_ERR_CTX_CHECK (ctx && ctx->magic == IPMI_CTX_MAGIC);

  API_ERR_DEVICE_ALREADY_OPEN(ctx->type == IPMI_DEVICE_UNKNOWN);

  API_ERR_PARAMETERS(hostname
                     && !(username && strlen (username) > IPMI_MAX_USER_NAME_LENGTH)
                     && !(password && strlen (password) > IPMI_2_0_MAX_PASSWORD_LENGTH)
                     && !(k_g && k_g_len > IPMI_MAX_K_G_LENGTH)
                     && IPMI_PRIVILEGE_LEVEL_VALID (privilege_level)
                     && IPMI_CIPHER_SUITE_ID_SUPPORTED(cipher_suite_id));
   
  API_ERR_PARAMETERS(!(strlen(hostname) > MAXHOSTNAMELEN));

  API_ERR_PARAMETERS(!(workaround_flags & ~flags_mask));

  API_ERR_CLEANUP (!(ipmi_rmcpplus_init() < 0));

  ctx->type = IPMI_DEVICE_LAN_2_0;
  ctx->workaround_flags = workaround_flags;
  ctx->flags = flags;

#ifdef HAVE_FUNC_GETHOSTBYNAME_R_6
  memset(&hent, '\0', sizeof(struct hostent));
  API_ERR_HOSTNAME_INVALID_CLEANUP(!gethostbyname_r(hostname,
                                                    &hent,
                                                    buf,
                                                    GETHOSTBYNAME_AUX_BUFLEN,
                                                    &hptr,
                                                    &h_errnop));
  API_ERR_HOSTNAME_INVALID_CLEANUP(hptr);
#else  /* !HAVE_FUNC_GETHOSTBYNAME_R */
#error Additional threadsafe gethostbyname support needed
#endif /* !HAVE_FUNC_GETHOSTBYNAME_R */

  strncpy(ctx->io.outofband.hostname,
          hostname,
          MAXHOSTNAMELEN);

  ctx->io.outofband.remote_host.sin_family = AF_INET;
  ctx->io.outofband.remote_host.sin_port = htons(RMCP_AUX_BUS_SHUNT);
  ctx->io.outofband.remote_host.sin_addr = *(struct in_addr *) hptr->h_addr;
  
  memset(ctx->io.outofband.username, '\0', IPMI_MAX_USER_NAME_LENGTH+1);
  if (username)
    memcpy (ctx->io.outofband.username, 
	    username, 
	    strlen (username));

  memset(ctx->io.outofband.password, '\0', IPMI_2_0_MAX_PASSWORD_LENGTH+1);
  if (password)
    memcpy (ctx->io.outofband.password, 
	    password, 
	    strlen (password));

  ctx->io.outofband.privilege_level = privilege_level;
  ctx->io.outofband.session_timeout = (session_timeout ? session_timeout : IPMI_SESSION_TIMEOUT);
  ctx->io.outofband.retransmission_timeout = (retransmission_timeout ? retransmission_timeout : IPMI_RETRANSMISSION_TIMEOUT);

  API_ERR_PARAMETERS(ctx->io.outofband.retransmission_timeout < ctx->io.outofband.session_timeout);

  memset(ctx->io.outofband.k_g, '\0', IPMI_MAX_K_G_LENGTH);
  ctx->io.outofband.k_g_configured = 0;
  if (k_g && k_g_len)
    {
      memcpy(ctx->io.outofband.k_g,
             k_g,
             k_g_len);
      ctx->io.outofband.k_g_configured++;
    }

  ctx->io.outofband.cipher_suite_id = cipher_suite_id;

  memset(ctx->io.outofband.sik_key, '\0', IPMI_MAX_SIK_KEY_LENGTH);
  ctx->io.outofband.sik_key_ptr = ctx->io.outofband.sik_key;
  ctx->io.outofband.sik_key_len = IPMI_MAX_SIK_KEY_LENGTH;
  memset(ctx->io.outofband.integrity_key, '\0', IPMI_MAX_INTEGRITY_KEY_LENGTH);
  ctx->io.outofband.integrity_key_ptr = ctx->io.outofband.integrity_key;
  ctx->io.outofband.integrity_key_len = IPMI_MAX_INTEGRITY_KEY_LENGTH;
  memset(ctx->io.outofband.confidentiality_key, '\0', IPMI_MAX_CONFIDENTIALITY_KEY_LENGTH);
  ctx->io.outofband.confidentiality_key_ptr = ctx->io.outofband.confidentiality_key;
  ctx->io.outofband.confidentiality_key_len = IPMI_MAX_CONFIDENTIALITY_KEY_LENGTH;
  memset(&ctx->io.outofband.last_send, '\0', sizeof(struct timeval));
  memset(&ctx->io.outofband.last_received, '\0', sizeof(struct timeval));

  ctx->io.outofband.highest_received_sequence_number = 0;
  ctx->io.outofband.previously_received_list = 0xFF;

  API_FIID_OBJ_CREATE_CLEANUP (ctx->io.outofband.rq.obj_rmcp_hdr, tmpl_rmcp_hdr);
  API_FIID_OBJ_CREATE_CLEANUP (ctx->io.outofband.rq.obj_lan_session_hdr, tmpl_lan_session_hdr);
  API_FIID_OBJ_CREATE_CLEANUP (ctx->io.outofband.rq.obj_rmcpplus_session_hdr, tmpl_rmcpplus_session_hdr);
  API_FIID_OBJ_CREATE_CLEANUP (ctx->io.outofband.rq.obj_lan_msg_hdr, tmpl_lan_msg_hdr_rq);
  API_FIID_OBJ_CREATE_CLEANUP (ctx->io.outofband.rq.obj_rmcpplus_session_trlr, tmpl_rmcpplus_session_trlr);
  
  API_FIID_OBJ_CREATE_CLEANUP (ctx->io.outofband.rs.obj_rmcp_hdr, tmpl_rmcp_hdr);
  API_FIID_OBJ_CREATE_CLEANUP (ctx->io.outofband.rs.obj_lan_session_hdr, tmpl_lan_session_hdr);
  API_FIID_OBJ_CREATE_CLEANUP (ctx->io.outofband.rs.obj_rmcpplus_session_hdr, tmpl_rmcpplus_session_hdr);
  API_FIID_OBJ_CREATE_CLEANUP (ctx->io.outofband.rs.obj_lan_msg_hdr, tmpl_lan_msg_hdr_rs);
  API_FIID_OBJ_CREATE_CLEANUP (ctx->io.outofband.rs.obj_rmcpplus_payload, tmpl_rmcpplus_payload);
  API_FIID_OBJ_CREATE_CLEANUP (ctx->io.outofband.rs.obj_lan_msg_trlr, tmpl_lan_msg_trlr);
  API_FIID_OBJ_CREATE_CLEANUP (ctx->io.outofband.rs.obj_rmcpplus_session_trlr, tmpl_rmcpplus_session_trlr);
  
  /* Open client (local) UDP socket */
  /* achu: ephemeral ports are > 1023, so no way we will bind to an IPMI port */
  
  API_ERR_SYSTEM_ERROR_CLEANUP (!((ctx->io.outofband.sockfd = socket (AF_INET, SOCK_DGRAM, 0)) < 0));

  memset (&addr, 0, sizeof (struct sockaddr_in));
  addr.sin_family = AF_INET;
  addr.sin_port   = htons (0);
  addr.sin_addr.s_addr = htonl (INADDR_ANY);

  API_ERR_SYSTEM_ERROR_CLEANUP (!(bind(ctx->io.outofband.sockfd, 
                                       (struct sockaddr *)&addr,
                                       sizeof(struct sockaddr_in)) < 0));
  
  /* errnum set in ipmi_lan_2_0_open_session */
  if (ipmi_lan_2_0_open_session (ctx) < 0)
    goto cleanup;

  ctx->errnum = IPMI_ERR_SUCCESS;
  return (0);

 cleanup:
  if (ctx->io.outofband.sockfd)
    close (ctx->io.outofband.sockfd);
  _ipmi_outofband_free (ctx);
  ctx->type = IPMI_DEVICE_UNKNOWN;
  return (-1);
}

int
ipmi_ctx_open_inband (ipmi_ctx_t ctx,
                      ipmi_driver_type_t driver_type, 
                      int disable_auto_probe, 
                      uint16_t driver_address, 
                      uint8_t register_spacing,
                      char *driver_device, 
                      uint32_t workaround_flags,
                      uint32_t flags)
{
  struct ipmi_locate_info locate_info;
  uint32_t temp_flags = 0;

  API_ERR_CTX_CHECK (ctx && ctx->magic == IPMI_CTX_MAGIC);

  API_ERR_DEVICE_ALREADY_OPEN(ctx->type == IPMI_DEVICE_UNKNOWN);

  API_ERR_PARAMETERS(driver_type == IPMI_DEVICE_KCS
                     || driver_type == IPMI_DEVICE_SMIC
                     || driver_type == IPMI_DEVICE_BT
                     || driver_type == IPMI_DEVICE_SSIF
                     || driver_type == IPMI_DEVICE_OPENIPMI);

  /* No workaround flags currently supported */
  API_ERR_PARAMETERS(!(workaround_flags));

  ctx->io.inband.kcs_ctx = NULL;
  ctx->io.inband.ssif_ctx = NULL;
  ctx->io.inband.openipmi_ctx = NULL;

  switch (driver_type)
    {
    case IPMI_DEVICE_KCS:
      if (disable_auto_probe)
	{
	  memset(&locate_info, '\0', sizeof(struct ipmi_locate_info));

	  locate_info.ipmi_version_major = 1;
	  locate_info.ipmi_version_minor = 5;
	  locate_info.locate_driver_type = IPMI_LOCATE_DRIVER_NONE;
	  locate_info.interface_type = IPMI_INTERFACE_KCS;
	  if (driver_device)
	    {
	      strncpy(locate_info.driver_device, driver_device, IPMI_LOCATE_PATH_MAX);
	      locate_info.driver_device[IPMI_LOCATE_PATH_MAX - 1] = '\0';
	    }
	  locate_info.address_space_id = IPMI_ADDRESS_SPACE_ID_SYSTEM_IO;
	  locate_info.driver_address = driver_address;
	  locate_info.register_spacing = register_spacing;
	}
      else 
	{
	  API_ERR_LOCATE_CLEANUP (ipmi_locate_get_device_info (IPMI_INTERFACE_KCS, 
                                                               &locate_info));
	  if (driver_device)
	    {
	      strncpy(locate_info.driver_device, 
                      driver_device, 
                      IPMI_LOCATE_PATH_MAX);
	      locate_info.driver_device[IPMI_LOCATE_PATH_MAX - 1] = '\0';
	    }
	  if (driver_address)
	    locate_info.driver_address = driver_address;
	  if (register_spacing)
	    locate_info.register_spacing = register_spacing;
	}
      ctx->type = driver_type;
      ctx->workaround_flags = workaround_flags;
      ctx->flags = flags;
      
      /* At this point we only support SYSTEM_IO, i.e. inb/outb style IO. 
	 If we cant find the bass address, we better exit. -- Anand Babu */
      API_ERR_DEVICE_NOT_SUPPORTED_CLEANUP (locate_info.address_space_id == IPMI_ADDRESS_SPACE_ID_SYSTEM_IO);
      
      API_ERR_CLEANUP ((ctx->io.inband.kcs_ctx = ipmi_kcs_ctx_create()));
      
      API_ERR_KCS_CLEANUP (!(ipmi_kcs_ctx_set_driver_address(ctx->io.inband.kcs_ctx, 
							     locate_info.driver_address) < 0));

      API_ERR_KCS_CLEANUP (!(ipmi_kcs_ctx_set_register_spacing(ctx->io.inband.kcs_ctx, 
                                                               locate_info.register_spacing) < 0));
      
      API_ERR_KCS_CLEANUP (!(ipmi_kcs_ctx_set_poll_interval(ctx->io.inband.kcs_ctx, 
							    IPMI_POLL_INTERVAL_USECS) < 0));
      
      if (ctx->flags & IPMI_FLAGS_NONBLOCKING)
        temp_flags |= IPMI_KCS_FLAGS_NONBLOCKING;
      
      API_ERR_KCS_CLEANUP (!(ipmi_kcs_ctx_set_flags(ctx->io.inband.kcs_ctx, temp_flags) < 0));
      
      API_ERR_KCS_CLEANUP (!(ipmi_kcs_ctx_io_init(ctx->io.inband.kcs_ctx) < 0));

      break;
    case IPMI_DEVICE_SMIC:
      ctx->errnum = IPMI_ERR_DEVICE_NOT_SUPPORTED;
      goto cleanup;
    case IPMI_DEVICE_BT:
      ctx->errnum = IPMI_ERR_DEVICE_NOT_SUPPORTED;
      goto cleanup;
    case IPMI_DEVICE_SSIF:
      if (disable_auto_probe)
	{
	  memset(&locate_info, '\0', sizeof(struct ipmi_locate_info));

	  locate_info.ipmi_version_major = 1;
	  locate_info.ipmi_version_minor = 5;
	  locate_info.locate_driver_type = IPMI_LOCATE_DRIVER_NONE;
	  locate_info.interface_type = IPMI_INTERFACE_SSIF;
	  strncpy(locate_info.driver_device, driver_device, IPMI_LOCATE_PATH_MAX);
	  locate_info.driver_device[IPMI_LOCATE_PATH_MAX - 1] = '\0';
	  locate_info.address_space_id = IPMI_ADDRESS_SPACE_ID_SMBUS;
	  locate_info.driver_address = driver_address;
	  locate_info.register_spacing = register_spacing;
	}
      else 
	{
	  API_ERR_LOCATE_CLEANUP (ipmi_locate_get_device_info (IPMI_INTERFACE_SSIF, 
                                                               &locate_info));
	  if (driver_device)
	    {
	      strncpy(locate_info.driver_device, 
                      driver_device, 
                      IPMI_LOCATE_PATH_MAX);
	      locate_info.driver_device[IPMI_LOCATE_PATH_MAX - 1] = '\0';
	    }
	  if (driver_address)
	    locate_info.driver_address = driver_address;
	  if (register_spacing)
	    locate_info.register_spacing = register_spacing;
	}
      ctx->type = driver_type;
      ctx->workaround_flags = workaround_flags;
      ctx->flags = flags;

      API_ERR_CLEANUP ((ctx->io.inband.ssif_ctx = ipmi_ssif_ctx_create()));
      
      API_ERR_SSIF_CLEANUP (!(ipmi_ssif_ctx_set_driver_device(ctx->io.inband.ssif_ctx, 
                                                              locate_info.driver_device) < 0));
 
      API_ERR_SSIF_CLEANUP (!(ipmi_ssif_ctx_set_driver_address(ctx->io.inband.ssif_ctx, 
                                                               locate_info.driver_address) < 0));

      if (ctx->flags & IPMI_FLAGS_NONBLOCKING)
        temp_flags |= IPMI_SSIF_FLAGS_NONBLOCKING;
      
      API_ERR_SSIF_CLEANUP (!(ipmi_ssif_ctx_set_flags(ctx->io.inband.ssif_ctx, temp_flags) < 0));

      API_ERR_SSIF_CLEANUP (!(ipmi_ssif_ctx_io_init(ctx->io.inband.ssif_ctx) < 0));

      break;

    case IPMI_DEVICE_OPENIPMI:
      ctx->type = driver_type;
      ctx->workaround_flags = workaround_flags;
      ctx->flags = flags;

      API_ERR_CLEANUP ((ctx->io.inband.openipmi_ctx = ipmi_openipmi_ctx_create()));
      
      if (driver_device)
        API_ERR_OPENIPMI_CLEANUP (!(ipmi_openipmi_ctx_set_driver_device(ctx->io.inband.openipmi_ctx,
                                                                        driver_device) < 0));
      
      API_ERR_OPENIPMI_CLEANUP (!(ipmi_openipmi_ctx_io_init(ctx->io.inband.openipmi_ctx) < 0));

      break;

    default:
      goto cleanup;
    }
  
  /* Prepare in-band headers */
  API_FIID_OBJ_CREATE_CLEANUP (ctx->io.inband.rq.obj_hdr, tmpl_hdr_kcs);
  API_FIID_OBJ_CREATE_CLEANUP (ctx->io.inband.rs.obj_hdr, tmpl_hdr_kcs);
  
  ctx->errnum = IPMI_ERR_SUCCESS;
  return (0);

 cleanup:
  _ipmi_inband_free (ctx);
  ctx->type = IPMI_DEVICE_UNKNOWN;
  return (-1);
}

int 
ipmi_cmd (ipmi_ctx_t ctx, 
	  uint8_t lun, 
	  uint8_t net_fn, 
	  fiid_obj_t obj_cmd_rq, 
	  fiid_obj_t obj_cmd_rs)
{
  int8_t status = 0;

  API_ERR_CTX_CHECK (ctx && ctx->magic == IPMI_CTX_MAGIC);

  API_ERR_DEVICE_NOT_OPEN(ctx->type != IPMI_DEVICE_UNKNOWN);

  API_ERR_INTERNAL_ERROR(ctx->type == IPMI_DEVICE_LAN
                         || ctx->type == IPMI_DEVICE_LAN_2_0
			 || ctx->type == IPMI_DEVICE_KCS
			 || ctx->type == IPMI_DEVICE_SSIF
			 || ctx->type == IPMI_DEVICE_OPENIPMI);

  API_FIID_OBJ_PACKET_VALID(obj_cmd_rq);

  ctx->lun = lun;
  ctx->net_fn = net_fn;
  
  if (ctx->flags & IPMI_FLAGS_DEBUG_DUMP)
    {
      /* lan packets are dumped in ipmi lan code */
      if (ctx->type != IPMI_DEVICE_LAN
          && ctx->type != IPMI_DEVICE_LAN_2_0)
        {
          char hdrbuf[DEBUG_COMMON_HDR_BUFLEN];
          uint64_t cmd = 0;

          API_FIID_OBJ_GET_NO_RETURN(obj_cmd_rq, "cmd", &cmd);

          debug_hdr_cmd(DEBUG_COMMON_TYPE_INBAND,
                        DEBUG_COMMON_DIRECTION_REQUEST,
                        ctx->net_fn,
                        cmd,
                        hdrbuf,
                        DEBUG_COMMON_HDR_BUFLEN);

          ipmi_obj_dump (STDERR_FILENO, 
                         NULL,
                         hdrbuf, 
                         NULL, 
                         obj_cmd_rq);
        }
    }

  if (ctx->type == IPMI_DEVICE_LAN)
    status = ipmi_lan_cmd (ctx, obj_cmd_rq, obj_cmd_rs);
  else if (ctx->type == IPMI_DEVICE_LAN_2_0)
    status = ipmi_lan_2_0_cmd (ctx, obj_cmd_rq, obj_cmd_rs);
  else if (ctx->type == IPMI_DEVICE_KCS)
    status = ipmi_kcs_cmd_api (ctx, obj_cmd_rq, obj_cmd_rs);
  else if (ctx->type == IPMI_DEVICE_SSIF)
    status = ipmi_ssif_cmd_api (ctx, obj_cmd_rq, obj_cmd_rs);
  else /* ctx->type == IPMI_DEVICE_OPENIPMI */
    status = ipmi_openipmi_cmd_api (ctx, obj_cmd_rq, obj_cmd_rs);
  
  if (ctx->flags & IPMI_FLAGS_DEBUG_DUMP)
    {
      /* lan packets are dumped in ipmi lan code */
      if (ctx->type != IPMI_DEVICE_LAN
          && ctx->type != IPMI_DEVICE_LAN_2_0)
        {
          char hdrbuf[DEBUG_COMMON_HDR_BUFLEN];
          uint64_t cmd = 0;
          
          API_FIID_OBJ_GET_NO_RETURN(obj_cmd_rq, "cmd", &cmd);

	  /* its ok to use the "request" net_fn */
          debug_hdr_cmd(DEBUG_COMMON_TYPE_INBAND,
                        DEBUG_COMMON_DIRECTION_RESPONSE,
                        ctx->net_fn,
                        cmd,
                        hdrbuf,
                        DEBUG_COMMON_HDR_BUFLEN);
          
          ipmi_obj_dump (STDERR_FILENO, 
                         NULL,
                         hdrbuf, 
                         NULL, 
                         obj_cmd_rs);
        }
    }

  /* errnum set in ipmi_*_cmd functions */
  return (status);
}

int 
ipmi_cmd_raw (ipmi_ctx_t ctx, 
              uint8_t lun,
              uint8_t net_fn,
	      uint8_t *in, 
	      size_t in_len, 
	      uint8_t *out, 
	      size_t out_len)
{
  int8_t status = 0;

  API_ERR_CTX_CHECK (ctx && ctx->magic == IPMI_CTX_MAGIC);

  API_ERR_DEVICE_NOT_OPEN(ctx->type != IPMI_DEVICE_UNKNOWN);

  if (ctx->type != IPMI_DEVICE_LAN
      && ctx->type != IPMI_DEVICE_LAN_2_0
      && ctx->type != IPMI_DEVICE_KCS
      && ctx->type != IPMI_DEVICE_SSIF
      && ctx->type != IPMI_DEVICE_OPENIPMI)
    {
      ctx->errnum = IPMI_ERR_INTERNAL_ERROR;
      return -1;
    }
  
  API_ERR_PARAMETERS(in
                     && in_len > 0
                     && out
                     && out_len > 0);

  ctx->lun = lun;
  ctx->net_fn = net_fn;

 if (ctx->type == IPMI_DEVICE_LAN)
   status = ipmi_lan_cmd_raw (ctx, in, in_len, out, out_len);
 else if (ctx->type == IPMI_DEVICE_LAN_2_0)
   status = ipmi_lan_2_0_cmd_raw (ctx, in, in_len, out, out_len);
 else if (ctx->type == IPMI_DEVICE_KCS)
   status = ipmi_kcs_cmd_raw_api (ctx, in, in_len, out, out_len);
 else if (ctx->type == IPMI_DEVICE_SSIF)
   status = ipmi_ssif_cmd_raw_api (ctx, in, in_len, out, out_len);
 else /* ctx->type == IPMI_DEVICE_OPENIPMI */
   status = ipmi_openipmi_cmd_raw_api (ctx, in, in_len, out, out_len);

  /* errnum set in ipmi_*_cmd_raw functions */
  return (status);
}

static void
_ipmi_outofband_close (ipmi_ctx_t ctx)
{
  /* Function Note: No need to set errnum - just return */
  assert(ctx
	 && ctx->magic == IPMI_CTX_MAGIC
	 && (ctx->type == IPMI_DEVICE_LAN
             || ctx->type == IPMI_DEVICE_LAN_2_0));
 
  /* No need to set errnum - if the anything in close session
   * fails, session will eventually timeout anyways
   */

  if (ipmi_lan_close_session (ctx) < 0)
    goto cleanup;

 cleanup:
  if (ctx->io.outofband.sockfd)
    close (ctx->io.outofband.sockfd);
  _ipmi_outofband_free (ctx);
}

static void
_ipmi_inband_close (ipmi_ctx_t ctx)
{
  /* Function Note: No need to set errnum - just return */
  assert(ctx 
	 && ctx->magic == IPMI_CTX_MAGIC
	 && (ctx->type == IPMI_DEVICE_KCS
	     || ctx->type == IPMI_DEVICE_SMIC
	     || ctx->type == IPMI_DEVICE_BT
	     || ctx->type == IPMI_DEVICE_SSIF
	     || ctx->type == IPMI_DEVICE_OPENIPMI));
  
  _ipmi_inband_free (ctx);
}

int 
ipmi_ctx_close (ipmi_ctx_t ctx)
{
  API_ERR_CTX_CHECK (ctx && ctx->magic == IPMI_CTX_MAGIC);

  API_ERR_DEVICE_NOT_OPEN(ctx->type != IPMI_DEVICE_UNKNOWN);

  if (ctx->type != IPMI_DEVICE_LAN
      && ctx->type != IPMI_DEVICE_LAN_2_0
      && ctx->type != IPMI_DEVICE_KCS
      && ctx->type != IPMI_DEVICE_SMIC
      && ctx->type != IPMI_DEVICE_BT
      && ctx->type != IPMI_DEVICE_SSIF
      && ctx->type != IPMI_DEVICE_OPENIPMI)
    {
      ctx->errnum = IPMI_ERR_INTERNAL_ERROR;
      return -1;
    }

  if (ctx->type == IPMI_DEVICE_LAN
      || ctx->type == IPMI_DEVICE_LAN_2_0)
    _ipmi_outofband_close (ctx);
  else
    _ipmi_inband_close (ctx);

  ctx->type = IPMI_DEVICE_UNKNOWN;
  ctx->errnum = IPMI_ERR_SUCCESS;
  return (0);
}

void
ipmi_ctx_destroy (ipmi_ctx_t ctx)
{
  ERR_VOID_RETURN (ctx && ctx->magic == IPMI_CTX_MAGIC);

  if (ctx->type != IPMI_DEVICE_UNKNOWN)
    ipmi_ctx_close(ctx);

  secure_memset(ctx, '\0', sizeof(ipmi_ctx_t));
  free(ctx);
}

int 
ipmi_ctx_set_debug_prefix (ipmi_ctx_t ctx, const char *prefix)
{
  API_ERR_CTX_CHECK (ctx && ctx->magic == IPMI_CTX_MAGIC);

  API_ERR_PARAMETERS(!(strlen(prefix) > MAXHOSTNAMELEN));

  if (prefix)
    {
      strncpy(ctx->debug_prefix, prefix, MAXHOSTNAMELEN);
      ctx->debug_prefix_set = 1;
    }
  else
    {
      memset(ctx->debug_prefix, '\0', MAXHOSTNAMELEN+1);
      ctx->debug_prefix_set = 0;
    }

  return 0;
}
