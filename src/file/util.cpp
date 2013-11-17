/*
  Copyright (C) 2007-2011 Sebastian Trueg <trueg@kde.org>

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU General Public License as
  published by the Free Software Foundation; either version 2 of
  the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Library General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this library; see the file COPYING.  If not, write to
  the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
  Boston, MA 02110-1301, USA.
*/

#include "util.h"
#include "database.h"

void Baloo::updateIndexingLevel(Database* db, int fileId, int level)
{
    try {
        Xapian::Document doc = db->xapainDatabase()->get_document(fileId);
        Xapian::TermIterator it = doc.termlist_begin();
        it.skip_to("Z");

        if (it != doc.termlist_end()) {
            std::string term = *it;
            if (term.find("Z"))
                doc.remove_term(term);
        }

        const QString term = QLatin1Char('Z') + QString::number(level);
        doc.add_term(term.toStdString());

        db->xapainDatabase()->replace_document(fileId, doc);
    }
    catch (const Xapian::DocNotFoundError&) {
        return;
    }
}