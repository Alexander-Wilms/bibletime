/*********
*
* This file is part of BibleTime's source code, http://www.bibletime.info/.
*
* Copyright 1999-2011 by the BibleTime developers.
* The BibleTime source code is licensed under the GNU General Public License version 2.0.
*
**********/

#ifndef BTCONFIG_H
#define BTCONFIG_H

#include <QDataStream>
#include <QHash>
#include <QMetaType>
#include <QSet>
#include <QSettings>
#include <QString>
#include <QStringList>
#include <QVariant>

#include "backend/config/btconfigcore.h"

#include "backend/btmoduletreeitem.h" // for BTModuleTreeItem::Grouping
#include "backend/drivers/cswordmoduleinfo.h"
#include "btglobal.h"
#include "frontend/searchdialog/btsearchoptionsarea.h" // for Search::BtSearchOptionsArea::SearchType


#define BTCONFIG_API_VERSION 1


class BibleTimeApp;

class BtConfig: public BtConfigCore {

    Q_DISABLE_COPY(BtConfig)

    friend class BibleTimeApp;

public: /* Types: */

    /*!
    * The first parameter indicates whether the custom font should be used or not.
    * The second parameter is the custom font that has been set.
    */
    typedef QPair<bool, QFont> FontSettingsPair;
    typedef QMap<QString, QString> StringMap;

private: /* Types: */

    typedef QHash<const CLanguageMgr::Language *, FontSettingsPair> FontCacheMap;

public: /* Methods: */

    static BtConfig & getInstance();

    /*!
     * \brief Function to set a module decryption key.
     *
     * This helper function will set a module decryption key
     * in the configuration. Any previous key will be overwritten.
     *
     * \param[in] name Name of module to set the key for
     * \param[in] key Decryption key to set as string
     */
    void setModuleEncryptionKey(const QString &name, const QString &key);

    /*!
     * \brief Function to get a module decryption key.
     *
     * This helper function will retrieve a previously set
     * module decryption key from the configuration. If the key
     * is not set it will return a null string.
     *
     * \param[in] name Name of module to retrieve the key for
     * \returns Decryption key as a string
     */
    QString getModuleEncryptionKey(const QString &name);

    /*!
     * \brief Gets the shortcuts for the given group.
     *
     * Returns a hash of shortcuts for strings for the respective
     * shortcut group.
     * \param[in] shortcutGroup The group to retrieve shortcuts for.
     * \returns Hash of strings and lists of shortcuts.
     */
    QHash< QString, QList<QKeySequence> > getShortcuts(const QString& shortcutGroup);

    /*!
     * \brief Sets the shortcuts for the given group.
     *
     * Writes a hash of shortcuts for strings for the respective
     * shortcut group.
     * \param[in] shortcutGroup The group to retrieve shortcuts for.
     * \param[in] Hash of strings and lists of shortcuts to write.
     */
    void setShortcuts( const QString& shortcutGroup, const QHash< QString, QList< QKeySequence > >& shortcuts);

    /*!
     * \brief Returns current filter options.
     *
     * Returns a structure containing the current
     * settings to be used for filtering.
     *
     * \returns FilterOptions structure containing filter settings.
     */
    FilterOptions getFilterOptions();

    /*!
     * \brief Saves the current filter options.
     *
     * \param[in] options The filter options to save.
     */
    void setFilterOptions(const FilterOptions & options);

    /*!
     * \brief Returns current display options.
     *
     * Returns a structure containing the current
     * settings to be used for displaying texts.
     *
     * \returns DisplayOptions structure containing display settings.
     */
    DisplayOptions getDisplayOptions();

    /*!
     * \brief Saves the current display options.
     *
     * \param[in] options The display options to save.
     */
    void setDisplayOptions(const DisplayOptions & options);

    /*!
     * Returns a default font that is suitable for the current language.
     * \returns QFont suitable for current language.
     */
    inline const QFont &getDefaultFont() const { return m_defaultFont; }

    /// \todo: put FontSettingsPair in QVariant directly
    /*!
     * \brief Set font for a language.
     *
     * Sets a FontSettingsPair for the language given.
     * \param[in] language pointer to a language to set the font for
     * \param[in] fontSettings FontSettingsPair to set
     */
    void setFontForLanguage(const CLanguageMgr::Language & language,
                            const FontSettingsPair & fontSettings);

    /*!
     * \brief Get font for a language.
     *
     * Gets a FontSettingsPair for the language given. If no font has been saved
     * a default font is returned.
     * \param[in] language pointer to a language to get the font for.
     * \returns FontSettingsPair for given language
     */
    FontSettingsPair getFontForLanguage(const CLanguageMgr::Language & language);

    /// \todo: unit test these functions
    /*!
     * Returns the searchScopes for the current locale.
     *
     * This function retrieves the search scopes of the
     * "properties/searchScopes" property and converts them
     * to the current locale.
     *
     * \returns Search scopes in current locale.
     */
    StringMap getSearchScopesForCurrentLocale();

    /*!
     * Sets the searchScopes given in the current locale.
     *
     * This function sets the search scopes of the
     * "properties/searchScopes" property, the scopes are
     * converted to the english locale before saving them.
     *
     * \param[in] searchScopes Search scopes in any locale.
     */
    void setSearchScopesWithCurrentLocale(StringMap searchScopes);

    /*!
      * Deletes the searchScopes given in the current locale.
      */
    void deleteSearchScopesWithCurrentLocale();

    /*!
     * \brief Returns default sword module info class for a given module type.
     *
     * This is basically a convenience function for getting the respective
     * "settings/defaults/ *" variable and searching that module manually.
     * If module is not installed 0 will be returned.
     * \param[in] moduleType module type to return the default sword module info for
     * \returns sword module info pointer or 0
     */
    CSwordModuleInfo* getDefaultSwordModuleByType(const QString& moduleType);

    /*!
     * \brief Sets the default sword module for a module type.
     *
     * This is basically a convenience function for setting the "settings/defaults/ *"
     * variables to the module->name() string manually.
     * 0 is allowed as the module, then the default module will be unset.
     * \param[in] moduleType module type to set
     * \param[in] module the sword module info to set as default module
     */
    void setDefaultSwordModuleByType(const QString& moduleType, const CSwordModuleInfo* const module);

private: /* Methods: */

    explicit BtConfig(const QString & settingsFile);

    static bool initBtConfig();

    static void destroyInstance();

private: /* Fields: */

    static BtConfig* m_instance; //!< singleton instance

    QFont m_defaultFont; //!< default font used when no special one is set
    FontCacheMap m_fontCache; //!< a cache for the fonts saved in the configuration file for speed

    static StringMap m_defaultSearchScopes;

};

// declare types used in configuration as metatype so they can be saved directly into the configuration
Q_DECLARE_METATYPE(BtConfig::StringMap)
Q_DECLARE_METATYPE(QList<int>)

/*!
 * \brief This is a shortchand for BtConfig::getInstance().
 * \returns BtConfig singleton instance.
 */
inline BtConfig &btConfig() {
    return BtConfig::getInstance();
}

#endif // BTCONFIG_H