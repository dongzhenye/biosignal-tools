// main_window_model.h

#ifndef MAIN_WINDOW_MODEL_IMPL_H
#define MAIN_WINDOW_MODEL_IMPL_H

#include "base/user_types.h"
#include "file_context.h"
#include "file_handling/event_manager.h"
#include "file_handling/channel_manager.h"
#include "gui/signal_visualisation_model.h"
#include "gui/main_window_model.h"

#include <memory>

#include <QObject>
#include <QStringList>
#include <QString>
#include <QSharedPointer>
#include <QTabWidget>

#include <list>
#include <map>
#include <set>

class QAction;
class QTextStream;

namespace BioSig_
{

class FileSignalReader;
class MainWindow;
class SignalBrowserModel;
class SignalBrowserView;
class EventTableFileReader;
class BlocksVisualisationView;
class AbstractBrowserModel;
class BlocksVisualisationModel;
class ApplicationContext;
class TabContext;

// main window model
class MainWindowModelImpl : public QObject, public MainWindowModel
{
    Q_OBJECT

public:
    MainWindowModelImpl ();
    ~MainWindowModelImpl();

    QTextStream& getLogStream();
    void setMainWindow(MainWindow* main_window);
    void loadSettings();
    void saveSettings();
    void setChanged();

    QSharedPointer<BlocksVisualisationModel> createBlocksVisualisationView (QString const& title);

    //-------------------------------------------------------------------------
    virtual QSharedPointer<SignalVisualisationModel> createSignalVisualisationOfFile (QSharedPointer<FileContext> file_ctx);

    //-------------------------------------------------------------------------
    virtual void closeCurrentFileTabs ();

    //-------------------------------------------------------------------------
    virtual QSharedPointer<SignalVisualisationModel> getCurrentSignalVisualisationModel ();
public slots:

    void tabChanged (int tab_index);
    void closeTab (int tab_index);

    // actions
    void calculateMeanAction();
    void calculateFrequencySpectrumAction();
    void undoViewAction();
    void redoViewAction();
    void undoAction();
    void redoAction();
    void fileSaveAction();
    void fileSaveAsAction();
    void fileExportEventsAction();
    void fileImportEventsAction();
    void fileCloseAction();
    void fileInfoAction();
    void fileExitAction();
    void viewZoomInAction();
    void viewZoomOutAction();
    void viewAutoScaleAction();
    void optionsChannelsAction();
    void optionsChangeCreationType();
    void optionsShowEventsAction();
    void optionsShowSettingsAction();
    void helpLogAction();
    void helpAboutAction();
    void recentFileActivated(QAction* recent_file_action);
    void recentFileMenuAboutToShow();


private:
    //-------------------------------------------------------------------------
    void storeAndInitTabContext (QSharedPointer<TabContext> context, int tab_index);

    //-------------------------------------------------------------------------
    bool checkMainWindowPtr(const QString function);

    static int const NUMBER_RECENT_FILES_;

    MainWindow* main_window_;
    QSharedPointer<ApplicationContext> application_context_;
    QSharedPointer<FileSignalReader> file_signal_reader_;
    QSharedPointer<FileContext> current_file_context_;
    QSharedPointer<SignalBrowserModel> signal_browser_model_;
    SignalBrowserView* signal_browser_; // main_window cares for destruction!!
    QTabWidget* tab_widget_;
    QWidget* signal_browser_tab_;
    QStringList recent_file_list_;
    std::auto_ptr<QTextStream> log_stream_;
    QString log_string_;
    bool overflow_detection_;
    std::map<int, QSharedPointer<AbstractBrowserModel> > browser_models_;
    std::list<QSharedPointer<BlocksVisualisationModel> > blocks_visualisation_models_;
    std::map<int, QSharedPointer<TabContext> > tab_contexts_;
    QSharedPointer<EventManager> event_manager_;
    QSharedPointer<ChannelManager> channel_manager_;

    //-------------------------------------------------------------------------
    // not allowed
    MainWindowModelImpl(const MainWindowModelImpl&);
    const MainWindowModelImpl& operator=(const MainWindowModelImpl&);

};

} // namespace BioSig_

#endif

