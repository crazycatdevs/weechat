/*
 * SPDX-FileCopyrightText: 2003-2025 Sébastien Helleu <flashcode@flashtux.org>
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

#ifndef WEECHAT_PLUGIN_FSET_BAR_ITEM_H
#define WEECHAT_PLUGIN_FSET_BAR_ITEM_H

#define FSET_BAR_ITEM_NAME "fset"

extern void fset_bar_item_update (void);
extern int fset_bar_item_init (void);
extern void fset_bar_item_end (void);

#endif /* WEECHAT_PLUGIN_FSET_BAR_ITEM_H */
