/*
 * SPDX-FileCopyrightText: 2013-2025 Sébastien Helleu <flashcode@flashtux.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
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
 * along with WeeChat.  If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef WEECHAT_SECURE_CONFIG_H
#define WEECHAT_SECURE_CONFIG_H

#define SECURE_CONFIG_NAME "sec"
#define SECURE_CONFIG_PRIO_NAME "120000|sec"

extern struct t_config_file *secure_config_file;
extern struct t_config_section *secure_config_section_pwd;

extern struct t_config_option *secure_config_crypt_cipher;
extern struct t_config_option *secure_config_crypt_hash_algo;
extern struct t_config_option *secure_config_crypt_passphrase_command;
extern struct t_config_option *secure_config_crypt_salt;

extern int secure_config_read (void);
extern int secure_config_write (void);
extern int secure_config_init (void);
extern void secure_config_free (void);

#endif /* WEECHAT_SECURE_CONFIG_H */
