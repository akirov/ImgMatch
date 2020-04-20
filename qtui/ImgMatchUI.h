#ifndef IMGMATCHUI_H
#define IMGMATCHUI_H

#include <list>
#if defined(__GXX_EXPERIMENTAL_CXX0X) || __cplusplus >= 201103L || (defined(_MSC_VER) && _MSC_VER >= 1900)
 #include <atomic>
#endif  // C++11

#include <QMainWindow>
//#include <QMetaType>  // For Q_DECLARE_METATYPE
#include <QThread>
#include <QMutex>
#include <QTimer>

#include "ImgMatch.h"


namespace Ui {
    class ImgMatchUI;
}


QT_BEGIN_NAMESPACE
class QAction;
class QMenu;
QT_END_NAMESPACE


struct ComPair 
{
    ComPair() :
        imgOneUri(""), imgTwoUri(""), compRes(-1)
    {}

    ComPair( const ComPair& other ) :
        imgOneUri(other.imgOneUri),
        imgTwoUri(other.imgTwoUri),
        compRes(other.compRes)
    {}

    ComPair( const std::string& s1, const std::string& s2, int res=-1 ) :
        imgOneUri(s1), imgTwoUri(s2), compRes(res)
    {}

    ~ComPair()
    {}

    std::string imgOneUri;
    std::string imgTwoUri;
    int         compRes;  // in %;  Or float in [0,1] range ???
};

//Q_DECLARE_METATYPE(ComPair);


class CompareThread;


class ImgMatchUI : public QMainWindow {

    Q_OBJECT

public:
    ImgMatchUI(QWidget *parent = 0);
    ~ImgMatchUI();

    typedef enum {
        SRC_INVALID=-1,
        SRC_ONE_DIR,
        SRC_TWO_DIR,
        SRC_IMG_DIR,
        SRC_TWO_IMG,
//      SRC_PROC_DIR,
        SRC_ONE_IMG
    } ImageSource;

private slots:
    void on_pbMoreRes_clicked();
    void on_pbDelImg1_clicked();
    void on_pbDelImg2_clicked();
    void on_pbViewClear_clicked();
    void on_pbViewDown_clicked();
    void on_pbViewUp_clicked();
    void on_twDupsTable_itemSelectionChanged();
    void on_pbFindStart_clicked();
    void on_rbSrcOneDir_clicked();
    void on_rbSrcTwoDir_clicked();
    void on_rbSrcImgDir_clicked();
    void on_rbSrcTwoImg_clicked();
    void on_rbProcImage_clicked();
    void on_pbSrcDir1_clicked();
    void on_pbSrcDir2_clicked();
    void on_pbSrcImg1_clicked();
    void on_pbSrcImg2_clicked();
    void on_actionExit_triggered();
    void about();
    void addRowInDupsTable(const ComPair& cmp);
    void addRowInResults(const ComPair& cmp);
    void progressUpdate(int complete);
    void numResultsUpdate();
    void compareFinished();
    void onTimerTick();
    void on_pbFindStop_clicked();  // Interface to stop compare thread

private:
    void createActions();
    void createMenus();
    void changeEvent(QEvent *e);
    void processSourceRB();
    void addNextResultsInDupsTable();

private:
    Ui::ImgMatchUI *ui;  // Or a unique_ptr, or not a pointer at all???

    QMenu *helpMenu;
    QAction *aboutAct;

    ImageSource mImgSrc;

    std::string mSrcDir1Name;
    std::string mSrcDir2Name;
    std::string mSrcImg1Name;
    std::string mSrcImg2Name;

    MatchMode mMatchMode;

    int mMatchThreshold;

    CompareThread* mComThread;
    QTimer* mQTimer;
    QMutex mMutex;

    std::list<ComPair> mResults;
    unsigned int mNumResults;
};


class CompareThread : public QThread
{
    Q_OBJECT

public:
    CompareThread( ImgMatchUI::ImageSource image_source, 
            const QString& src1_name, const QString& src2_name,
            MatchMode match_mode, int match_threshold, 
            QObject* parent = 0 );

    ~CompareThread();

    void run();
    int getItemsProc() const;

Q_SIGNALS:
    void sendProgressRange(int, int);  // Set progress bar range
    void sendRowInDupsTable(const ComPair& cmp);  // Update table
    void sendRowInResults(const ComPair& cmp);  // Update results. Or "ComPair cmp"?
    void sendNumResultsUpdate();
    void sendProgressUpdate(int);  // Update progress
    void sendCompareFinished();  // When compare is finished

private:
    ImgMatchUI::ImageSource mImageSource;
    QString                 mSrc1Name;
    QString                 mSrc2Name;
    MatchMode               mMatchMode;
    int                     mMatchThreshold;
#if defined(__GXX_EXPERIMENTAL_CXX0X) || __cplusplus >= 201103L || (defined(_MSC_VER) && _MSC_VER >= 1900)
    std::atomic<int>        mItProc;
#else
    // TODO add a lock
    int                     mItProc;
#endif  // C++11
};

#endif // IMGMATCHUI_H
