/*
 * SPDX-FileCopyrightText: 2019-2025 Sébastien Helleu <flashcode@flashtux.org>
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
 * along with WeeChat.  If not, see <http://www.gnu.org/licenses/>.
 */

/* Test IRC configuration functions */

#include "CppUTest/TestHarness.h"

extern "C"
{
#include "src/plugins/irc/irc-config.h"
}

TEST_GROUP(IrcConfig)
{
};

/*
 * Tests functions:
 *   irc_config_notice_nick_notify
 */

TEST(IrcConfig, NoticeNickNotify)
{
    LONGS_EQUAL(0, irc_config_notice_nick_notify (NULL));
    LONGS_EQUAL(1, irc_config_notice_nick_notify (""));

    LONGS_EQUAL(1, irc_config_notice_nick_notify ("test"));
    LONGS_EQUAL(1, irc_config_notice_nick_notify ("memoserv"));

    /* default list of nicks preventing notification */
    LONGS_EQUAL(0, irc_config_notice_nick_notify ("chanserv"));
    LONGS_EQUAL(0, irc_config_notice_nick_notify ("ChanServ"));
    LONGS_EQUAL(0, irc_config_notice_nick_notify ("nickserv"));
    LONGS_EQUAL(0, irc_config_notice_nick_notify ("NickServ"));
}

/*
 * Tests functions:
 *   irc_config_check_autojoin
 */

TEST(IrcConfig, CheckAutojoin)
{
    /* NULL/empty string */
    LONGS_EQUAL(1, irc_config_check_autojoin (NULL));
    LONGS_EQUAL(1, irc_config_check_autojoin (""));

    /* wrong delimiter: space instead of comma */
    LONGS_EQUAL(0, irc_config_check_autojoin ("#chan1 #chan2 #chan3"));

    /* no spaces allowed around comma */
    LONGS_EQUAL(0, irc_config_check_autojoin ("#chan1, #chan2"));
    LONGS_EQUAL(0, irc_config_check_autojoin ("#chan1 ,#chan2"));
    LONGS_EQUAL(0, irc_config_check_autojoin ("#chan1 , #chan2"));
    LONGS_EQUAL(0, irc_config_check_autojoin ("#chan1, #chan2, #chan3"));
    LONGS_EQUAL(0, irc_config_check_autojoin ("#chan1 ,#chan2 ,#chan3"));
    LONGS_EQUAL(0, irc_config_check_autojoin ("#chan1 , #chan2 , #chan3"));
    LONGS_EQUAL(0, irc_config_check_autojoin ("#chan1,#chan2 key1, key2"));
    LONGS_EQUAL(0, irc_config_check_autojoin ("#chan1,#chan2 key1 ,key2"));
    LONGS_EQUAL(0, irc_config_check_autojoin ("#chan1,#chan2 key1 , key2"));

    /* too many keys */
    LONGS_EQUAL(0, irc_config_check_autojoin ("#chan1,#chan2 key1,key2,key3"));

    /* correct values */
    LONGS_EQUAL(1, irc_config_check_autojoin ("#chan1"));
    LONGS_EQUAL(1, irc_config_check_autojoin ("#chan1 "));
    LONGS_EQUAL(1, irc_config_check_autojoin ("#chan1  "));
    LONGS_EQUAL(1, irc_config_check_autojoin (" #chan1"));
    LONGS_EQUAL(1, irc_config_check_autojoin ("  #chan1"));
    LONGS_EQUAL(1, irc_config_check_autojoin (" #chan1 "));
    LONGS_EQUAL(1, irc_config_check_autojoin ("  #chan1  "));
    LONGS_EQUAL(1, irc_config_check_autojoin ("#c1,#c2"));
    LONGS_EQUAL(1, irc_config_check_autojoin ("#c1,#c2,#c3"));
    LONGS_EQUAL(1, irc_config_check_autojoin ("#c1,#c2,#c3 key1"));
    LONGS_EQUAL(1, irc_config_check_autojoin ("#c1,#c2,#c3 key1,key2"));
    LONGS_EQUAL(1, irc_config_check_autojoin ("#c1,#c2,#c3 key1,key2,key3"));
}
