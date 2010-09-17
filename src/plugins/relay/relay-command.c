/*
 * Copyright (C) 2003-2010 Sebastien Helleu <flashcode@flashtux.org>
 *
 * This file is part of WeeChat, the extensible chat client.
 *
 * WeeChat is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * WeeChat is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with WeeChat.  If not, see <http://www.gnu.org/licenses/>.
 */

/*
 * relay-command.c: relay command
 */

#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "../weechat-plugin.h"
#include "relay.h"
#include "relay-buffer.h"
#include "relay-client.h"
#include "relay-config.h"
#include "relay-raw.h"
#include "relay-server.h"


/*
 * relay_command_client_list: list clients
 */

void
relay_command_client_list (int full)
{
    struct t_relay_client *ptr_client;
    int i;
    char date_start[128], date_activity[128];
    struct tm *date_tmp;
    
    if (relay_clients)
    {
        weechat_printf (NULL, "");
        weechat_printf (NULL, _("Clients for relay:"));
        i = 1;
        for (ptr_client = relay_clients; ptr_client;
             ptr_client = ptr_client->next_client)
        {
            date_tmp = localtime (&(ptr_client->start_time));
            strftime (date_start, sizeof (date_start),
                      "%a, %d %b %Y %H:%M:%S", date_tmp);
            
            date_tmp = localtime (&(ptr_client->last_activity));
            strftime (date_activity, sizeof (date_activity),
                      "%a, %d %b %Y %H:%M:%S", date_tmp);
                
            if (full)
            {
                weechat_printf (NULL,
                                _("  id: %d, %s%s%s (%s%s%s), "
                                  "started on: %s, last activity: %s, "
                                  "bytes: %lu recv, %lu sent"),
                                ptr_client->id,
                                RELAY_COLOR_CHAT_HOST,
                                ptr_client->address,
                                RELAY_COLOR_CHAT,
                                RELAY_COLOR_CHAT_BUFFER,
                                relay_client_status_string[ptr_client->status],
                                RELAY_COLOR_CHAT,
                                date_start,
                                date_activity,
                                ptr_client->bytes_recv,
                                ptr_client->bytes_sent);
            }
            else
            {
                if (!RELAY_CLIENT_HAS_ENDED(ptr_client->status))
                {
                    weechat_printf (NULL,
                                    _("  id: %d, %s%s%s, started on: %s"),
                                    ptr_client->id,
                                    RELAY_COLOR_CHAT_HOST,
                                    ptr_client->address,
                                    RELAY_COLOR_CHAT,
                                    date_start);
                }
            }
            i++;
        }
    }
    else
        weechat_printf (NULL, _("No client for relay"));
}

/*
 * relay_command_server_list: list servers (list of port on which we are
 *                            listening)
 */

void
relay_command_server_list ()
{
    struct t_relay_server *ptr_server;
    int i;
    char date_start[128];
    struct tm *date_tmp;
    
    if (relay_servers)
    {
        weechat_printf (NULL, "");
        weechat_printf (NULL, _("Listening on ports:"));
        i = 1;
        for (ptr_server = relay_servers; ptr_server;
             ptr_server = ptr_server->next_server)
        {
            date_tmp = localtime (&(ptr_server->start_time));
            strftime (date_start, sizeof (date_start),
                      "%a, %d %b %Y %H:%M:%S", date_tmp);
            
            weechat_printf (NULL,
                            _("  port %s%d%s, relay: %s%s.%s%s, started on: %s"),
                            RELAY_COLOR_CHAT_BUFFER,
                            ptr_server->port,
                            RELAY_COLOR_CHAT,
                            RELAY_COLOR_CHAT_BUFFER,
                            relay_protocol_string[ptr_server->protocol],
                            ptr_server->protocol_string,
                            RELAY_COLOR_CHAT,
                            date_start);
            i++;
        }
    }
    else
        weechat_printf (NULL, _("No server for relay"));
}

/*
 * relay_command_relay: command /relay
 */

int
relay_command_relay (void *data, struct t_gui_buffer *buffer, int argc,
                     char **argv, char **argv_eol)
{
    struct t_relay_server *ptr_server;
    
    /* make C compiler happy */
    (void) data;
    (void) buffer;
    (void) argv_eol;

    if (argc > 1)
    {
        if (weechat_strcasecmp (argv[1], "list") == 0)
        {
            relay_command_client_list (0);
            return WEECHAT_RC_OK;
        }
        
        if (weechat_strcasecmp (argv[1], "listfull") == 0)
        {
            relay_command_client_list (1);
            return WEECHAT_RC_OK;
        }
        
        if (weechat_strcasecmp (argv[1], "listrelay") == 0)
        {
            relay_command_server_list ();
            return WEECHAT_RC_OK;
        }
        if (weechat_strcasecmp (argv[1], "add") == 0)
        {
            if (argc >= 4)
            {
                relay_config_create_option_port (NULL,
                                                 relay_config_file,
                                                 relay_config_section_port,
                                                 argv[2],
                                                 argv_eol[3]);
            }
            else
            {
                weechat_printf (NULL,
                            _("%s%s: missing arguments for \"%s\" "
                              "command"),
                            weechat_prefix ("error"), RELAY_PLUGIN_NAME,
                            "relay add");
            }
            return WEECHAT_RC_OK;
        }
        if (weechat_strcasecmp (argv[1], "del") == 0)
        {
            if (argc >= 3)
            {
                ptr_server = relay_server_search (argv_eol[2]);
                if (ptr_server)
                {
                    relay_server_free (ptr_server);
                }
                else
                {
                    weechat_printf (NULL,
                                    _("%s%s: relay \"%s\" not found"),
                                    weechat_prefix ("error"),
                                    RELAY_PLUGIN_NAME,
                                    argv_eol[2]);
                }
            }
            else
            {
                weechat_printf (NULL,
                                _("%s%s: missing arguments for \"%s\" "
                                  "command"),
                                weechat_prefix ("error"), RELAY_PLUGIN_NAME,
                                "relay add");
            }
            return WEECHAT_RC_OK;
        }
        if (weechat_strcasecmp (argv[1], "raw") == 0)
        {
            relay_raw_open (1);
            return WEECHAT_RC_OK;
        }
    }
    
    if (!relay_buffer)
        relay_buffer_open ();
    
    if (relay_buffer)
    {
        weechat_buffer_set (relay_buffer, "display", "1");
        
        if (argc > 1)
        {
            if (strcmp (argv[1], "up") == 0)
            {
                if (relay_buffer_selected_line > 0)
                    relay_buffer_selected_line--;
            }
            else if (strcmp (argv[1], "down") == 0)
            {
                if (relay_buffer_selected_line < relay_client_count - 1)
                    relay_buffer_selected_line++;
            }
        }
    }
    
    relay_buffer_refresh (NULL);
    
    return WEECHAT_RC_OK;
}

/*
 * relay_command_init: add /relay command
 */

void
relay_command_init ()
{
    weechat_hook_command ("relay",
                          N_("relay control"),
                          N_("[list | listfull | add protocol.name port | "
                             "del protocol.name | raw]"),
                          N_("         list: list relay clients (only active "
                             "relays)\n"
                             "     listfull: list relay clients (verbose, all "
                             "relays)\n"
                             "    listrelay: list relays (name and port)\n"
                             "          add: add relay for a protocol + name\n"
                             "          del: remove relay for a protocol + name\n"
                             "protocol.name: protocol and name to relay\n"
                             "               for example: irc.freenode\n"
                             "         port: port used for relay\n"
                             "          raw: open buffer with raw Relay data\n\n"
                             "Without argument, this command opens buffer "
                             "with list of relay clients."),
                          "list %(relay_relays)"
                          " || listfull %(relay_relays)"
                          " || listrelay"
                          " || add %(relay_protocol_name) %(relay_free_port)"
                          " || del %(relay_relays)"
                          " || raw",
                          &relay_command_relay, NULL);
}
