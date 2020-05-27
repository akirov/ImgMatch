#include <sstream>
#include <iomanip>

#include "qglobal.h"
#if QT_VERSION >= 0x050000
    #include <QtWidgets>
#else
    #include <QtGui>
#endif  // QT_VERSION
#include <QDir>
#include <QFileInfo>


#include "ImgMatchUI.h"
#include "ui_ImgMatchUI.h"
#include "ModScale.h"
#include "Logger.h"


ImgMatchUI::ImgMatchUI(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ImgMatchUI),
    mImgSrc(SRC_INVALID),
    mMatchMode(MOD_INVALID),
    mMatchThreshold(0),
    mComThread(NULL),
    mQTimer(NULL),
    mMutex(),
    mResults(),
    mNumResults(0)
{
    ui->setupUi(this);

    createActions();
    createMenus();

    processSourceRB();
}


ImgMatchUI::~ImgMatchUI()
{
    LOG("Destroying ImgMatchUI");

    delete ui;
}


void ImgMatchUI::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}


void ImgMatchUI::processSourceRB()
{
    bool dir1en, dir2en, img1en, img2en;

    dir1en = dir2en = img1en = img2en = false;

    if( ui->rbSrcOneDir->isChecked() )
    {
        mImgSrc = SRC_ONE_DIR;

        dir1en = true;
        dir2en = false;
        img1en = false;
        img2en = false;
    }
    else if( ui->rbSrcTwoDir->isChecked() )
    {
        mImgSrc = SRC_TWO_DIR;

        dir1en = true;
        dir2en = true;
        img1en = false;
        img2en = false;
    }
    else if( ui->rbSrcImgDir->isChecked() )
    {
        mImgSrc = SRC_IMG_DIR;

        dir1en = true;
        dir2en = false;
        img1en = true;
        img2en = false;
    }
    else if( ui->rbSrcTwoImg->isChecked() )
    {
        mImgSrc = SRC_TWO_IMG;

        dir1en = false;
        dir2en = false;
        img1en = true;
        img2en = true;
    }
    else if( ui->rbProcImage->isChecked() )
    {
        mImgSrc = SRC_ONE_IMG;

        dir1en = false;
        dir2en = false;
        img1en = true;
        img2en = false;
    }
    else
    {
        mImgSrc = SRC_INVALID;
    }

    ui->lbSrcDir1->setEnabled(dir1en);
    ui->leSrcDir1->setEnabled(dir1en);
    ui->pbSrcDir1->setEnabled(dir1en);

    ui->lbSrcDir2->setEnabled(dir2en);
    ui->leSrcDir2->setEnabled(dir2en);
    ui->pbSrcDir2->setEnabled(dir2en);

    ui->lbSrcImg1->setEnabled(img1en);
    ui->leSrcImg1->setEnabled(img1en);
    ui->pbSrcImg1->setEnabled(img1en);

    ui->lbSrcImg2->setEnabled(img2en);
    ui->leSrcImg2->setEnabled(img2en);
    ui->pbSrcImg2->setEnabled(img2en);
}


void ImgMatchUI::createActions()
{
    aboutAct = new QAction(tr("&About"), this);
    connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));
}


void ImgMatchUI::createMenus()
{
    helpMenu = new QMenu(tr("&Help"), this);
    helpMenu->addAction(aboutAct);

    menuBar()->addMenu(helpMenu);
}


void ImgMatchUI::about()
{
    QMessageBox::about(this, tr("About ImgMatch"),
            tr("<p><b>ImgMatch</b> finds similar images</p>"));
}


void ImgMatchUI::on_actionExit_triggered()
{
    if ( mComThread )
    {
        mComThread->requestInterruption();
        mComThread->wait(); // Block until the thread is done completely.
        delete mComThread;
        mComThread = NULL;
    }

    close();
}


void ImgMatchUI::closeEvent(QCloseEvent *event)
{
    if ( mComThread )
    {
        mComThread->requestInterruption();
        mComThread->wait(); // Block until the thread is done completely.
        delete mComThread;
        mComThread = NULL;
    }

    event->accept();
}


void ImgMatchUI::on_pbSrcDir1_clicked()
{
    QString dirName;

    dirName = QFileDialog::getExistingDirectory(this, "Choose a directory");

    if ( ! dirName.isEmpty() )
    {
        ui->leSrcDir1->setText( dirName );

        if ( (mImgSrc == SRC_ONE_DIR)
          || ((mImgSrc == SRC_TWO_DIR) && (! ui->leSrcDir2->text().isEmpty()))
          || ((mImgSrc == SRC_IMG_DIR) && (! ui->leSrcImg1->text().isEmpty())) )
            ui->pbFindStart->setEnabled(true);
    }
}


void ImgMatchUI::on_pbSrcDir2_clicked()
{
    QString dirName;

    dirName = QFileDialog::getExistingDirectory(this, "Choose a directory");

    if ( ! dirName.isEmpty() )
    {
        ui->leSrcDir2->setText( dirName );

        if ( (mImgSrc == SRC_TWO_DIR) && (! ui->leSrcDir1->text().isEmpty()) )
            ui->pbFindStart->setEnabled(true);
    }
}


void ImgMatchUI::on_pbSrcImg1_clicked()
{
    QString fileName;

    fileName = QFileDialog::getOpenFileName(
        this,
        "Choose a file to open",
        QString::null,
        QString::null);

    if ( ! fileName.isEmpty() )
    {
        ui->leSrcImg1->setText( fileName );

        if ( ((mImgSrc == SRC_TWO_IMG) && (! ui->leSrcImg2->text().isEmpty()))
          || ((mImgSrc == SRC_IMG_DIR) && (! ui->leSrcDir1->text().isEmpty()))
          || (mImgSrc == SRC_ONE_IMG))
            ui->pbFindStart->setEnabled(true);
    }
}


void ImgMatchUI::on_pbSrcImg2_clicked()
{
    QString fileName;

    fileName = QFileDialog::getOpenFileName(
        this,
        "Choose a file to open",
        QString::null,
        QString::null);

    if ( ! fileName.isEmpty() )
    {
        ui->leSrcImg2->setText( fileName );

        if ( (mImgSrc == SRC_TWO_IMG) && (! ui->leSrcImg1->text().isEmpty()) )
            ui->pbFindStart->setEnabled(true);
    }
}


void ImgMatchUI::on_rbSrcOneDir_clicked()
{
    processSourceRB();
}


void ImgMatchUI::on_rbSrcTwoDir_clicked()
{
    processSourceRB();
}


void ImgMatchUI::on_rbSrcImgDir_clicked()
{
    processSourceRB();
}


void ImgMatchUI::on_rbSrcTwoImg_clicked()
{
    processSourceRB();
}


void ImgMatchUI::on_rbProcImage_clicked()
{
    processSourceRB();
}


void ImgMatchUI::addRowInDupsTable( const ComPair& cmp )
{
    std::stringstream ss;
    ss << std::setw(5) << cmp.compRes << "%";

    QTableWidgetItem* item[3];
    item[0] = new QTableWidgetItem(QString::fromStdString(cmp.imgOneUri));
    item[1] = new QTableWidgetItem(QString::fromStdString(ss.str()));
    item[2] = new QTableWidgetItem(QString::fromStdString(cmp.imgTwoUri));

    int row = ui->twDupsTable->rowCount(); // current row count
#if 0
    ui->twDupsTable->setRowCount(row+1);
#else
    ui->twDupsTable->insertRow(row);
#endif /* 0 */

    bool sorting = ui->twDupsTable->isSortingEnabled();
    if( sorting ) ui->twDupsTable->setSortingEnabled(false);

    ui->twDupsTable->setItem(row, 0, item[0]);
    item[1]->setTextAlignment(Qt::AlignRight);
    ui->twDupsTable->setItem(row, 1, item[1]);
    ui->twDupsTable->setItem(row, 2, item[2]);

    if( sorting ) ui->twDupsTable->setSortingEnabled(true);
}


void ImgMatchUI::progressUpdate( int progress )
{
    ui->progressBar->setValue(progress);
    onTimerTick();
}


#define ORDERED_INSERT 0

void ImgMatchUI::addRowInResults( const ComPair& cmp )
{
    QMutexLocker ml(&mMutex);
#if ORDERED_INSERT
    if ( mResults.size() == 0 )
        mResults.push_front(cmp);
    else if ( cmp.compRes <= mResults.end()->compRes )
        mResults.push_back(cmp);
    else  // Put it in the right place
    {
        for ( std::list<ComPair>::iterator it=mResults.begin(); it != mResults.end(); it++ )
        {
            if ( cmp.compRes > it->compRes )
            {
                mResults.insert(it, cmp);
                break;
            }
        }
    }
#else  // sequential insert
    mResults.push_back(cmp);
#endif // ordered/sequential insert
    mNumResults++;
//    ui->lbNumRes->setText(QString::number(mNumResults));
}


void ImgMatchUI::numResultsUpdate()
{
    ui->lbNumRes->setText(QString::number(mNumResults));
}


#define RESULTS_AT_ONCE 100

void ImgMatchUI::addNextResultsInDupsTable()
{
    QMutexLocker ml(&mMutex);
    int numResToAdd = std::min(RESULTS_AT_ONCE, (int)mResults.size());

    for( int i=0; i<numResToAdd; ++i )
    {
        std::list<ComPair>::iterator pos=mResults.begin();
#if !ORDERED_INSERT
        int maxRes = 0;

        for ( std::list<ComPair>::iterator it=mResults.begin(); it != mResults.end(); it++ )
        {
            if( it->compRes > maxRes )
            {
                maxRes = it->compRes;
                pos = it;
            }
        }
#endif // ORDERED_INSERT
        addRowInDupsTable(*pos);
        mResults.erase(pos);
    }

    // Disable "Show more" after processing has finished and all results displayed
    if ( (mComThread == NULL) && (numResToAdd < RESULTS_AT_ONCE) )
        ui->pbMoreRes->setEnabled(false);
}


void ImgMatchUI::compareFinished()
{
    if ( mQTimer )
    {
        mQTimer->stop();
        delete mQTimer;
        mQTimer = NULL;
    }
    onTimerTick();

    if ( mComThread )
    {
        mComThread->wait(); // Block until the thread is done completely.
        delete mComThread;
        mComThread = NULL;
    }

    LOG("Compare finish");

    // Enable "Start" find button
    ui->pbFindStart->setEnabled(true);

    // Disable "Stop" find button
    ui->pbFindStop->setEnabled(false);

    // Disable progress bar
    ui->progressBar->setEnabled(false);

    // Disable processed items label
    ui->lbItemsProc->setEnabled(false);

    ui->lbNumRes->setText(QString::number(mNumResults));

    addNextResultsInDupsTable();
}


void ImgMatchUI::onTimerTick()
{
    if ( ! mComThread ) return;
    int i = mComThread->getItemsProc();
    ui->lbItemsProc->setText(QString::number(i) + QString(" items processed"));
}


/******************************************************************************/
// TODO: move in a separate file. Rename to CompareManagerThread?


CompareThread::CompareThread( ImgMatchUI::ImageSource image_source, 
        const QString& src1_name, const QString& src2_name,
        MatchMode match_mode, int match_threshold, 
        QObject* parent ) :
    QThread(parent),
    mImageSource(image_source),
    mSrc1Name(src1_name),
    mSrc2Name(src2_name),
    mMatchMode(match_mode),
    mMatchThreshold(match_threshold),
    mItProc(0)
{
}


CompareThread::~CompareThread()
{
    LOG("Destroying CompareThread");
}


void CompareThread::run()
{
    LOG("CompareThread Id : " << QThread::currentThreadId());

    mItProc = 0;  // TODO lock #if !defined(__GXX_EXPERIMENTAL_CXX0X) && __cplusplus < 201103L && (!defined(_MSC_VER) || _MSC_VER < 1900)

//  exec();  // Starts event loop. Does not return until exit(). We only need this to process signals.

#if defined(__GXX_EXPERIMENTAL_CXX0X) || __cplusplus >= 201103L || (defined(_MSC_VER) && _MSC_VER >= 1900)
    std::unique_ptr<ImgMatch> img_match(nullptr);
#else
    std::auto_ptr<ImgMatch> img_match(NULL);
#endif  // C++11

    switch ( mMatchMode )  // TODO use a factory
    {
        case MOD_SCALE:
            img_match.reset( new ModScale );
            break;

        default:
            THROW("Match mode " << mMatchMode << " is not implemented");
    }

    switch( mImageSource )
    {
        case ImgMatchUI::SRC_ONE_DIR:
        {
            QDir dir1(mSrc1Name);
            QStringList file_list, filters;
            filters << "*.bmp" << "*.dib" << "*.jpeg" << "*.jpg" << "*.jpe" <<
                "*.png" << "*.pbm" << "*.pgm" << "*.ppm" << "*.tiff" << "*.tif";
            file_list = dir1.entryList (filters, QDir::Files | QDir::Hidden);

            int N = file_list.size();

            if ( N < 2 ) 
                break;

            int numPairs = (N*(N-1))/2;
            int progress_update_interval = numPairs/50;

            if ( progress_update_interval < 1 )
                progress_update_interval = 1;

            // Init the progress bar
            Q_EMIT sendProgressRange(0, numPairs);

            // Cycle over the image pairs to_compare, calling Compare() method for each
            // of them. Update the progress bar. Break if the Stop button is pressed.
            // Add each processed pair to the results, if it is above match threshold.

            int itProc=0;
            for ( int i=0; !isInterruptionRequested() && i<(N-1); i++ )
            {
                // If i == 0 print status "Caching..." else print "Comparing..."?
                for ( int j=i+1; !isInterruptionRequested() && j<N; j++ )
                {
                    /* TODO: only push work items in a work queue and process them
                     * in a pool of worker threads */
                    ComPair cmp(mSrc1Name.toStdString() + "/" + file_list[i].toStdString(), 
                                mSrc1Name.toStdString() + "/" + file_list[j].toStdString());

                    cmp.compRes = 100 * img_match->Compare(cmp.imgOneUri, cmp.imgTwoUri);

                    if ( (!mMatchThreshold) || (cmp.compRes >= mMatchThreshold) )
                    {
#if 0
                        Q_EMIT sendRowInDupsTable(cmp);
#else
                        Q_EMIT sendRowInResults(cmp);
#endif // 0
                        Q_EMIT sendNumResultsUpdate();
                    }

#if defined(__GXX_EXPERIMENTAL_CXX0X) || __cplusplus >= 201103L || (defined(_MSC_VER) && _MSC_VER >= 1900)
                    itProc = std::atomic_fetch_add(&mItProc, 1) + 1;
#else
                    // TODO lock
                    itProc = ++mItProc;
#endif  // C++11

                    // Update the progress bar
                    if ( (itProc % progress_update_interval) == 0 )
                    {
                        Q_EMIT sendProgressUpdate(itProc);
                    }
                }
            }
            Q_EMIT sendProgressUpdate(itProc);

            break;
        }

        case ImgMatchUI::SRC_TWO_DIR:
        {
            QDir dir1(mSrc1Name);
            QDir dir2(mSrc2Name);
            QStringList file_list1, file_list2, filters;
            filters << "*.bmp" << "*.dib" << "*.jpeg" << "*.jpg" << "*.jpe" <<
                "*.png" << "*.pbm" << "*.pgm" << "*.ppm" << "*.tiff" << "*.tif";
            file_list1 = dir1.entryList (filters, QDir::Files | QDir::Hidden);
            file_list2 = dir2.entryList (filters, QDir::Files | QDir::Hidden);

            int N1 = file_list1.size();
            int N2 = file_list2.size();

            if ( (N1 < 1) || (N2 < 1) )
                // Emit an error?
                break;

            int numPairs = N1 * N2;
            int progress_update_interval = numPairs/50;

            if ( progress_update_interval < 1 )
                progress_update_interval = 1;

            // Init the progress bar
            Q_EMIT sendProgressRange(0, numPairs);

            int itProc=0;
            for ( int i=0; !isInterruptionRequested() && i<N1; i++ )
            {
                // If i == 0 print status "Caching..." else print "Comparing..."?
                for ( int j=0; !isInterruptionRequested() && j<N2; j++ )
                {
                    ComPair cmp(mSrc1Name.toStdString() + "/" + file_list1[i].toStdString(),
                                mSrc2Name.toStdString() + "/" + file_list2[j].toStdString());

                    cmp.compRes = 100 * img_match->Compare(cmp.imgOneUri, cmp.imgTwoUri);

                    if ( (!mMatchThreshold) || (cmp.compRes >= mMatchThreshold) )
                    {
#if 0
                        Q_EMIT sendRowInDupsTable(cmp);
#else
                        Q_EMIT sendRowInResults(cmp);
#endif // 0
                        Q_EMIT sendNumResultsUpdate();
                    }

#if defined(__GXX_EXPERIMENTAL_CXX0X) || __cplusplus >= 201103L || (defined(_MSC_VER) && _MSC_VER >= 1900)
                    itProc = std::atomic_fetch_add(&mItProc, 1) + 1;
#else
                    // TODO lock
                    itProc = ++mItProc;
#endif  // C++11

                    // Update the progress bar
                    if ( (itProc % progress_update_interval) == 0 )
                    {
                        Q_EMIT sendProgressUpdate(itProc);
                    }
                }
            }
            Q_EMIT sendProgressUpdate(itProc);

            break;
        }

        case ImgMatchUI::SRC_TWO_IMG:
        {
            // Init the progress bar
            Q_EMIT sendProgressRange(0, 1);

            ComPair cmp(mSrc1Name.toStdString(), 
                        mSrc2Name.toStdString());

            cmp.compRes = 100 * img_match->Compare(cmp.imgOneUri, cmp.imgTwoUri);

//            if ( (!mMatchThreshold) || (cmp.compRes >= mMatchThreshold) )
            {
#if 0
                Q_EMIT sendRowInDupsTable(cmp);
#else
                Q_EMIT sendRowInResults(cmp);
#endif // 0
                Q_EMIT sendNumResultsUpdate();
            }

            mItProc = 1;  // TODO lock #if !defined(__GXX_EXPERIMENTAL_CXX0X) && __cplusplus < 201103L && (!defined(_MSC_VER) || _MSC_VER < 1900)

            // Update the progress bar
            Q_EMIT sendProgressUpdate(1);

            break;
        }

        default:
            THROW( "Unknown source!" );
    }

    Q_EMIT sendCompareFinished();
}


int CompareThread::getItemsProc() const
{
#if defined(__GXX_EXPERIMENTAL_CXX0X) || __cplusplus >= 201103L || (defined(_MSC_VER) && _MSC_VER >= 1900)
    return mItProc.load();  // Add std::memory_order_acquire ?
#else
    // TODO: Lock!
    return mItProc;
#endif  // C++11
}


/******************************************************************************/


void ImgMatchUI::on_pbFindStart_clicked()
{
    QString src1name, src2name;

    // Take the source
    switch( mImgSrc )
    {
        case SRC_ONE_DIR:
        {
            src1name = ui->leSrcDir1->text();
            mSrcDir1Name = src1name.toStdString();
            break;
        }

        case SRC_TWO_DIR:
        {
            src1name = ui->leSrcDir1->text();
            src2name = ui->leSrcDir2->text();
            if ( src1name == src2name )
            {
                QMessageBox::warning(this, "Warning", "Two dirs must be different");
                return;
            }
            mSrcDir1Name = src1name.toStdString();
            mSrcDir2Name = src2name.toStdString();
            break;
        }

        case SRC_IMG_DIR:
        {
            src1name = ui->leSrcDir1->text();
            mSrcDir1Name = src1name.toStdString();
            src2name = ui->leSrcImg1->text();
            mSrcImg1Name = src2name.toStdString();
            break;
        }

        case SRC_TWO_IMG:
        {
            src1name = ui->leSrcImg1->text();
            mSrcImg1Name = src1name.toStdString();
            src2name = ui->leSrcImg2->text();
            mSrcImg2Name = src2name.toStdString();
            break;
        }

        case SRC_ONE_IMG:
        {
            src1name = ui->leSrcImg1->text();
            mSrcImg1Name = src1name.toStdString();
            break;
        }

        default:
            THROW( "Unknown source!" );
    }

    // Take the method
    if( ui->rbMetColDist->isChecked() )
    {
        mMatchMode = MOD_SCALE;
    }
    else if( ui->rbMetText->isChecked() )
    {
        mMatchMode = MOD_TEXT;
    }

    // Take the match threshold value
    mMatchThreshold = ui->spinBox->value();

    // Disable "Start" find button
    ui->pbFindStart->setEnabled(false);

    // Enable "Stop" find button
    ui->pbFindStop->setEnabled(true);

    // Enable progress bar
    ui->progressBar->setEnabled(true);

    // Enable processed items label
    ui->lbItemsProc->setEnabled(true);

    // Reset the results
    on_pbViewClear_clicked();

    // Enable "Show More" results button
    ui->pbMoreRes->setEnabled(true);

    // Enable "View dups" tab. Or do it only upon completion?
    ui->tabView->setEnabled(true);

    if ( mMatchMode == MOD_TEXT )  // Process individual images
    {
        // TODO
    }
    else  // Compare images
    {
        LOG("Compare start. Main threadId = " << QThread::currentThreadId());

        /* Add "this" as parent to delete CompareThread when ImgMatchUI is deleted.
         * Or connect &CompareThread::finished with &QObject::deleteLater ? */
        mComThread = new CompareThread(mImgSrc, src1name, src2name, mMatchMode, mMatchThreshold, this);

        // Connect signals and slots
        connect(mComThread, SIGNAL(sendProgressRange(int, int)), ui->progressBar, SLOT(setRange(int, int)));
#if 0  // Update progress bar directly
        connect(mComThread, SIGNAL(sendProgressUpdate(int)), ui->progressBar, SLOT(setValue(int)));
#else
        connect(mComThread, SIGNAL(sendProgressUpdate(int)), this, SLOT(progressUpdate(int)));
#endif // 0

        qRegisterMetaType<ComPair>("ComPair");  // Or qRegisterMetaType<ComPair>(); with Q_DECLARE_METATYPE(ComPair);
        connect(mComThread, SIGNAL(sendRowInDupsTable(ComPair)), this, SLOT(addRowInDupsTable(ComPair)));
        // With Qt::DirectConnection the slot is called in CompareThread context, otherwise - in GUI thread!
        connect(mComThread, SIGNAL(sendRowInResults(ComPair)), this, SLOT(addRowInResults(ComPair)), Qt::DirectConnection);
        connect(mComThread, SIGNAL(sendNumResultsUpdate()), this, SLOT(numResultsUpdate()));

        connect(mComThread, SIGNAL(sendCompareFinished()), this, SLOT(compareFinished()));

        // No need for this - it will be connected utomatically
        //connect(ui->pbFindStop, SIGNAL(clicked()), this, SLOT(on_pbFindStop_clicked()));

        mComThread->start();  // Calls run(). Set QThread::LowPriority?

        mQTimer = new QTimer(this);
        mQTimer->setInterval(2000);
        connect(mQTimer, SIGNAL(timeout()), this, SLOT(onTimerTick()));
        mQTimer->start();
    }
}


void ImgMatchUI::on_pbFindStop_clicked()
{
    LOG("Stop button clicked, threadId = " << QThread::currentThreadId());

    if( NULL != mComThread ) mComThread->requestInterruption();
}


void ImgMatchUI::on_twDupsTable_itemSelectionChanged()
{
    QList<QTableWidgetItem *> selItems = ui->twDupsTable->selectedItems();

    if ( selItems.size() == 0 )
        return;

    QTableWidgetItem *item = selItems[0];
    QString fileName = item->data(Qt::DisplayRole).toString();

    QImage image1(fileName);
    if (image1.isNull())
    {
        ui->qlImgLabel1->setText("Image not found");
    }
    else
    {
        std::stringstream imgInfo;
        imgInfo << image1.width() << "x" << image1.height();
        imgInfo << "  " << std::setprecision(1) << std::fixed
                << QFileInfo(fileName).size()/1024.0 << "K";

        QSize size = ui->qlImgLabel1->size();
        image1 = image1.scaled(size, Qt::KeepAspectRatio, Qt::SmoothTransformation);

        QPainter* painter = new QPainter(&image1);  // Or use a stack object???
        painter->setPen(Qt::white);
        painter->setFont(QFont("Arial", 10));
        painter->drawText(image1.rect(), Qt::AlignTop | Qt::AlignLeft, QString::fromStdString(imgInfo.str()));
        delete painter;  // ???

        ui->qlImgLabel1->setPixmap(QPixmap::fromImage(image1));
    }

    // Display image1 name
    ui->leImgInfo1->setText(fileName);

    if ( mMatchMode == MOD_TEXT )
    {
        // TODO
    }
    else
    {
#if 0
        // Enable "Delete1" button
        if ( !image1.isNull() && !ui->pbDelImg1->isEnabled() )
            ui->pbDelImg1->setEnabled(true);
#endif // 0

        item = selItems[2];
        fileName = item->data(Qt::DisplayRole).toString();

        QImage image2 = QImage(fileName);
        if (image2.isNull())
        {
            ui->qlImgLabel2->setText("Image not found");
        }
        else
        {
            std::stringstream imgInfo;
            imgInfo << image2.width() << "x" << image2.height();
            imgInfo << "  " << std::setprecision(1) << std::fixed
                    << QFileInfo(fileName).size()/1024.0 << "K";

            QSize size = ui->qlImgLabel2->size();
            image2 = image2.scaled(size, Qt::KeepAspectRatio, Qt::SmoothTransformation);

            QPainter* painter = new QPainter(&image2);  // Or use a stack object???
            painter->setPen(Qt::white);
            painter->setFont(QFont("Arial", 10));
            painter->drawText(image2.rect(), Qt::AlignTop | Qt::AlignLeft, QString::fromStdString(imgInfo.str()));
            delete painter;  // ???

            ui->qlImgLabel2->setPixmap(QPixmap::fromImage(image2));
        }

        // Display image2 name
        ui->leImgInfo2->setText(fileName);
#if 0
        // Enable "Delete2" button
        if ( !image2.isNull() && !ui->pbDelImg2->isEnabled() )
            ui->pbDelImg2->setEnabled(true);
#endif // 0
    }
}


void ImgMatchUI::on_pbViewUp_clicked()
{
    int cur_row = ui->twDupsTable->currentRow();

    if( cur_row > 0 ) {
        cur_row--;
    }

    ui->twDupsTable->setCurrentCell( cur_row, 0, QItemSelectionModel::SelectCurrent
                                               | QItemSelectionModel::Rows);
}


void ImgMatchUI::on_pbViewDown_clicked()
{
    int cur_row = ui->twDupsTable->currentRow();

    if( cur_row < ui->twDupsTable->rowCount() - 1 ) {
        cur_row++;
    }

    ui->twDupsTable->setCurrentCell( cur_row, 0, QItemSelectionModel::SelectCurrent
                                               | QItemSelectionModel::Rows);
}


void ImgMatchUI::on_pbViewClear_clicked()
{
    ui->pbDelImg1->setEnabled(false);
    ui->qlImgLabel1->clear();
    ui->leImgInfo1->clear();

    ui->pbDelImg2->setEnabled(false);
    ui->qlImgLabel2->clear();
    ui->leImgInfo2->clear();

    ui->twDupsTable->clearContents();
    ui->twDupsTable->setRowCount(0);

    ui->lbNumRes->setText("0");
    mResults.clear();
    mNumResults = 0;

    if ( mMatchMode == MOD_TEXT )
    {
        ui->qlImgLabel2->setStyleSheet("QLabel { background-color : white; color : black; }");
        // TODO
    }
    else
    {
        if (ui->twDupsTable->columnCount() < 3)
            ui->twDupsTable->setColumnCount(3);

        ui->twDupsTable->setHorizontalHeaderItem(0, new QTableWidgetItem("First Image"));
        ui->twDupsTable->setHorizontalHeaderItem(1, new QTableWidgetItem("% Match"));
        ui->twDupsTable->setHorizontalHeaderItem(2, new QTableWidgetItem("Second Image"));

        // Need to do this on resize too
        int dupsTableWidth = ui->twDupsTable->width();
        ui->twDupsTable->setColumnWidth(0, dupsTableWidth*0.40);
        ui->twDupsTable->setColumnWidth(1, dupsTableWidth*0.10);
        ui->twDupsTable->setColumnWidth(2, dupsTableWidth*0.40);
    }

    ui->progressBar->setValue(0);
    ui->lbItemsProc->setText("0 items processed");
}


void ImgMatchUI::on_pbDelImg1_clicked()
{
    // TODO
}


void ImgMatchUI::on_pbDelImg2_clicked()
{
    // TODO
}


void ImgMatchUI::on_pbMoreRes_clicked()
{
    addNextResultsInDupsTable();
}
