#ifndef DOCUMENTCONTROLLER_H
#define DOCUMENTCONTROLLER_H


#include <QObject>
#include <QPair>
#include <QStringList>

#include "settingscontroller.h"


namespace QSint
{


class MainWindow;
class DocumentFactory;
class Document;
struct DocTypeInfo;
struct DocFileInfo;
class CreateDialogBase;
class OpenDialogBase;


class DocumentController : public QObject, public Restorable
{
    Q_OBJECT

    typedef MainWindow ParentClass;

public:
    typedef QPair<DocumentFactory*, const DocFileInfo*> DocFileTypeIndex;

    enum LastDirPolicy
    {
        LDP_FIXED,          /// always open the same directory
        LDP_LAST,           /// always open last directory of the current operation
        LDP_LAST_ANY,       /// always open last directory of any previous operation
        LDP_LAST_IGNORED    /// always open last directory of any previous operation except current
    };

    explicit DocumentController(ParentClass *parent);

    // Called once when initialized
    virtual void init();

    // Serialization
    virtual bool store(QSettings& set);
    virtual bool restore(QSettings& set);

    // File actions availability
    virtual bool canNewFile() const;
    virtual bool canOpenFile() const;
    virtual bool canReloadFile() const;
    virtual bool canSaveFile() const;
    virtual bool canSaveFileAs() const;
    virtual bool canSaveAllFiles() const;

    // Last dir policies management
    void setLastOpenDir(const QString& dirName, LastDirPolicy policy = LDP_LAST_ANY);
    void setLastStoreDir(const QString& dirName, LastDirPolicy policy = LDP_LAST_ANY);

    // Set create file dialog
    void setCreateDialog(CreateDialogBase* dialog, bool deletePrevious = true);

    // Set open file dialog
    void setOpenDialog(OpenDialogBase* dialog, bool deletePrevious = true);

    // Document factories management
    virtual bool addFactory(DocumentFactory* factory);

    // Documents
    const QList<Document*>& documents() const
            { return m_documents; }

    Document* documentOpened(const QString& filename) const;
    bool isDocumentOpened(const QString& filename) const
            { return documentOpened(filename) != NULL; }

Q_SIGNALS:
    void changed();
    void documentCreated(Document*);
    void documentChanged(Document*);

public Q_SLOTS:
    // File actions processing
    virtual void createFile();
    virtual void openFile();

protected:
    // Shows dialog for new document type
    virtual const DocTypeInfo* chooseNewDocumentType(const QList<const DocTypeInfo*>& docTypes);

    // Shows dialog for opening files
    virtual QStringList chooseFilesToOpen(const QList<DocFileTypeIndex>& docFilters,
                                          const QString& rootDir,
                                          bool allowAllFiles,
                                          int* filterIndex = 0);

    // Creates documents from the given files in \a filesList via factory with given index \a filterIndex
    virtual void createDocuments(const QStringList& filesList,
                                 int filterIndex = -1);

    // Reloads documents in \a docList
    virtual void reloadDocuments(const QList<Document*>& docList);

    // Shows dialog with already opened files in \a filesList.
    // Returns files to reload (i.e. the same \a filesList by default),
    // or empty list if reload has not been accepted.
    virtual QStringList showAlreadyOpenedFiles(const QStringList& filesList);

    // Shows dialog with not opened files in \a filesList.
    // Returns files to try to open again (i.e. the same \a filesList by default),
    // or empty list if another try has not been accepted.
    virtual QStringList showNotOpenedFiles(const QStringList& filesList);

protected:
    QList<DocumentFactory*> m_factories;
    QList<Document*> m_documents;

    QList<const DocTypeInfo*> m_createDocTypes;
    QList<DocFileTypeIndex> m_openDocTypes;

    bool m_allowAllFilesFilter;

    QString m_lastOpenDir;
    LastDirPolicy m_lastOpenDirPolicy;
    QString m_lastStoreDir;
    LastDirPolicy m_lastStoreDirPolicy;

    CreateDialogBase* m_createDialog;
    OpenDialogBase* m_openDialog;
};


}


#endif // DOCUMENTCONTROLLER_H