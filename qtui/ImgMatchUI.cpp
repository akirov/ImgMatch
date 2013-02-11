#include <sstream>
#include <iomanip>

#include <QtGui>
#include <QDir>
#include <QFileInfo>


#include "ImgMatchUI.h"
#include "ui_ImgMatchUI.h"
#include "ModScale.h"
#include "QtImage.h"
#include "Logger.h"


ImgMatchUI::ImgMatchUI(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ImgMatchUI),
    mImgSrc(SRC_INVALID),
    mMatchMode(MOD_INVALID),
    mMatchThreshold(0),
    mStopFlag(false),
    mComThread(NULL)
{
    ui->setupUi(this);

    createActions();
    createMenus();

    processSourceRB();
}


ImgMatchUI::~ImgMatchUI()
{
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
    close();
}


void ImgMatchUI::on_pbSrcDir1_clicked()
{
    QString dirName;

    dirName = QFileDialog::getExistingDirectory(this, "Choose a directory");

    if ( ! dirName.isEmpty() )
    {
        ui->leSrcDir1->setText( dirName );

        if ( (mImgSrc == SRC_ONE_DIR)
          or ((mImgSrc == SRC_TWO_DIR) and (! ui->leSrcDir2->text().isEmpty()))
          or ((mImgSrc == SRC_IMG_DIR) and (! ui->leSrcImg1->text().isEmpty())) )
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

        if ( (mImgSrc == SRC_TWO_DIR) and (! ui->leSrcDir1->text().isEmpty()) )
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

        if ( ((mImgSrc == SRC_TWO_IMG) and (! ui->leSrcImg2->text().isEmpty()))
          or ((mImgSrc == SRC_IMG_DIR) and (! ui->leSrcDir1->text().isEmpty())) )
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

        if ( (mImgSrc == SRC_TWO_IMG) and (! ui->leSrcImg1->text().isEmpty()) )
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


void ImgMatchUI::addRowInDupsTable( const ComPair& cmp )
{
    LOG("ImgMatchUI::addRowInDupsTable()");

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
}


void ImgMatchUI::compareFinished()
{
    // Enable "Start" find button
    ui->pbFindStart->setEnabled(true);

    // Disable "Stop" find button
    ui->pbFindStop->setEnabled(false);

    // Upon compare finish enable "View dups"
    ui->tabView->setEnabled(true);

    int dupsTableWidth = ui->twDupsTable->width();
    ui->twDupsTable->setColumnWidth(0, dupsTableWidth*0.40);
    ui->twDupsTable->setColumnWidth(1, dupsTableWidth*0.10);
    ui->twDupsTable->setColumnWidth(2, dupsTableWidth*0.40);

    // Disable Delete buttons until image is selected
    ui->pbDelImg1->setEnabled(false);
    ui->pbDelImg2->setEnabled(false);
}


#ifndef PROCESSING_THREAD

void ImgMatchUI::compareProcess( ImageSource image_source, 
        const QString& src1_name, const QString& src2_name,
        MatchMode match_mode, int match_threshold, 
        int progress_update_interval )
{
    std::auto_ptr<ImgMatch> img_match(NULL);

    switch ( match_mode )
    {
        case MOD_SCALE_DOWN:
            img_match.reset( new ModScale );
            break;

        default:
            THROW("Match mode " << match_mode << " is not implemented");
    }

    switch( image_source )
    {
        case SRC_ONE_DIR:
        {
            QDir dir1(src1_name);
            QStringList file_list, filters;
            filters << "*.jpg" << "*.jpeg";
            file_list = dir1.entryList (filters, QDir::Files | QDir::Hidden);

            int N = file_list.size();

            if ( N < 2 ) 
                break;  // Or return?

            // Init the progress bar
            ui->progressBar->setRange(0, (N*(N-1))/2);

            // Cycle over the image pairs to_compare, calling Compare() method for each
            // of them. Update the progress bar. Break if the Stop button is pressed.
            // Add each processed pair to ViewDups table.

            int progress = 0;

            if ( (N*(N-1))/2 < 10 )
                progress_update_interval = 1;
            else
                progress_update_interval = (N*(N-1))/20;
            
            for ( int i=0; !mStopFlag && i<(N-1); i++ )
            {
                for ( int j=i+1; !mStopFlag && j<N; j++ )
                {
                    ComPair cmp(src1_name.toStdString() + "/" + file_list[i].toStdString(), 
                                src1_name.toStdString() + "/" + file_list[j].toStdString());

                    QtImage img1(cmp.imgOneUri);
                    QtImage img2(cmp.imgTwoUri);
                    // Check for errors when opening the images!

                    cmp.compRes = 100 * img_match->Compare(img1, img2);

//                  if ( match_threshold && cmp.compRes >= match_threshold )
                        addRowInDupsTable(cmp);

                    ++progress;

                    QApplication::processEvents();

                    // Update the progress bar
                    if ( (progress % progress_update_interval) == 0 )
                    {
                        progressUpdate(progress);
                    }
                }
            }
            progressUpdate(progress);

            break;
        }

        case SRC_TWO_IMG:
        {
            // Init the progress bar
            ui->progressBar->setRange(0, 1);

            ComPair cmp(src1_name.toStdString(), 
                        src2_name.toStdString());

            QtImage img1(cmp.imgOneUri);
            QtImage img2(cmp.imgTwoUri);
            // Check for errors when opening the images!

            cmp.compRes = 100 * img_match->Compare(img1, img2);

            addRowInDupsTable(cmp);

            // Update the progress bar
            progressUpdate(1);

            break;
        }

        default:
            THROW( "Unknown source!" );
    }

    compareFinished();
}

#else

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
    mStopFlag(false)
{
}


void CompareThread::run()
{
//  exec();  // Starts event loop. Do we need this???

    std::auto_ptr<ImgMatch> img_match(NULL);

    switch ( mMatchMode )
    {
        case MOD_SCALE_DOWN:
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
            filters << "*.jpg" << "*.jpeg";
            file_list = dir1.entryList (filters, QDir::Files | QDir::Hidden);
            int progress_update_interval;

            int N = file_list.size();

            if ( N < 2 ) 
                break;  // Or return?

            // Init the progress bar
            Q_EMIT sendProgressRange(0, (N*(N-1))/2);

            // Cycle over the image pairs to_compare, calling Compare() method for each
            // of them. Update the progress bar. Break if the Stop button is pressed.
            // Add each processed pair to ViewDups table.

            int progress = 0;

            if ( (N*(N-1))/2 < 10 )
                progress_update_interval = 1;
            else
                progress_update_interval = (N*(N-1))/20;
            
            for ( int i=0; !mStopFlag && i<(N-1); i++ )
            {
                for ( int j=i+1; !mStopFlag && j<N; j++ )
                {
                    ComPair cmp(mSrc1Name.toStdString() + "/" + file_list[i].toStdString(), 
                                mSrc1Name.toStdString() + "/" + file_list[j].toStdString());

                    QtImage img1(cmp.imgOneUri);
                    QtImage img2(cmp.imgTwoUri);
                    // Check for errors when opening the images!

                    cmp.compRes = 100 * img_match->Compare(img1, img2);

//                  if ( mMatchThreshold && cmp.compRes >= mMatchThreshold )
                        Q_EMIT sendRowInDupsTable(cmp);

                    ++progress;

                    // Update the progress bar
                    if ( (progress % progress_update_interval) == 0 )
                    {
                        Q_EMIT sendProgressUpdate(progress);
                    }
                }
            }
            Q_EMIT sendProgressUpdate(progress);

            break;
        }

        case ImgMatchUI::SRC_TWO_IMG:
        {
            // Init the progress bar
            Q_EMIT sendProgressRange(0, 1);

            ComPair cmp(mSrc1Name.toStdString(), 
                        mSrc2Name.toStdString());

            QtImage img1(cmp.imgOneUri);
            QtImage img2(cmp.imgTwoUri);
            // Check for errors when opening the images!

            cmp.compRes = 100 * img_match->Compare(img1, img2);

            Q_EMIT sendRowInDupsTable(cmp);

            // Update the progress bar
            Q_EMIT sendProgressUpdate(1);

            break;
        }

        default:
            THROW( "Unknown source!" );
    }

    Q_EMIT sendCompareFinished();
}

#endif // PROCESSING_THREAD


void ImgMatchUI::on_pbFindStart_clicked()
{
    QString src1name, src2name;

    switch( mImgSrc )
    {
        case SRC_ONE_DIR:
        {
            src1name = ui->leSrcDir1->text();
            mSrcDir1Name = src1name.toStdString();
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

        default:
            THROW( "Unknown source!" );
    }

    // Take the method
    if( ui->rbMetScale->isChecked() )
    {
        mMatchMode = MOD_SCALE_DOWN;
    }
    else if( ui->rbMetSig->isChecked() )
    {
        mMatchMode = MOD_IMG_SIG;
    }
    // ...

    // Take the mach threshold value
    mMatchThreshold = ui->spinBox->value();

    // Disable "Start" find button
    ui->pbFindStart->setEnabled(false);

    // Enable "Stop" find button
    ui->pbFindStop->setEnabled(true);
    mStopFlag = false;

#ifndef PROCESSING_THREAD
    compareProcess( mImgSrc, src1name, src2name, mMatchMode, mMatchThreshold );
#else
    mComThread = new CompareThread(mImgSrc, src1name, src2name, mMatchMode, mMatchThreshold);  // XXX Create it dynamicaly! Not on the stack!

    // Connect signals and slots
    connect(mComThread, SIGNAL(sendProgressRange(int, int)), ui->progressBar, SLOT(setRange(int, int)));
    connect(mComThread, SIGNAL(sendProgressUpdate(int)), ui->progressBar, SLOT(setValue(int)));

    qRegisterMetaType<ComPair>("ComPair");  // Or qRegisterMetaType<ComPair>(); with Q_DECLARE_METATYPE(ComPair);
    connect(mComThread, SIGNAL(sendRowInDupsTable(ComPair)), this, SLOT(addRowInDupsTable(ComPair)));

    connect(mComThread, SIGNAL(sendCompareFinished()), this, SLOT(compareFinished()));

    connect(ui->pbFindStop, SIGNAL(clicked()), mComThread, SLOT(on_pbFindStop_clicked()));

    mComThread->start();  // Calls run()

//  mComThread->wait(); // Block until the thread is done? Don't need this.
#endif // PROCESSING_THREAD
}


#ifdef PROCESSING_THREAD
void CompareThread::on_pbFindStop_clicked()
#else
void ImgMatchUI::on_pbFindStop_clicked()
#endif // PROCESSING_THREAD
{
    mStopFlag = true;
    LOG("mStopFlag = true");
}


void ImgMatchUI::on_twDupsTable_itemSelectionChanged()
{
    QList<QTableWidgetItem *> selItems = ui->twDupsTable->selectedItems();

    if ( selItems.size() == 0 )
        return;

    QTableWidgetItem *item = selItems[0];
    QString fileName = item->data(Qt::DisplayRole).toString();

    QImage image1(fileName);
    if (image1.isNull()) {
        QMessageBox::information(this, tr("InOut"),
                                 tr("Cannot load %1.").arg(fileName));
        return;
    }

    std::stringstream dim;
    dim << image1.width() << "x" << image1.height();

    QSize size = ui->qlImgLabel1->size();
    image1 = image1.scaled(size, Qt::KeepAspectRatio, Qt::SmoothTransformation);

    QPainter* painter = new QPainter(&image1);
    painter->setPen(Qt::white);
    painter->setFont(QFont("Arial", 10));
    painter->drawText(image1.rect(), Qt::AlignTop | Qt::AlignLeft, QString::fromStdString(dim.str()));
    delete painter;  // ???

//  QLabel* qlImgLabel1 = new QLabel;
//  qlImgLabel1->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
//  qlImgLabel1->setScaledContents(false);
//  ui->saViewImg1->setWidget(qlImgLabel1);
    ui->saViewImg1->setBackgroundRole(QPalette::Dark);

    ui->qlImgLabel1->setPixmap(QPixmap::fromImage(image1));

    // Display image1 name and size
    std::stringstream name_size;
    name_size << fileName.toStdString() << "  " << std::setprecision(1) 
              << std::fixed << QFileInfo(fileName).size()/1024.0 << "K";
    ui->leImgInfo1->setText(name_size.str().c_str());

    // Enable "Delete1" button
    if ( ! ui->pbDelImg1->isEnabled() )
        ui->pbDelImg1->setEnabled(true);


    item = selItems[2];
    fileName = item->data(Qt::DisplayRole).toString();

    QImage image2 = QImage(fileName);
    if (image2.isNull()) {
        QMessageBox::information(this, tr("InOut"),
                                 tr("Cannot load %1.").arg(fileName));
        return;
    }

    dim.str("");
    dim << image2.width() << "x" << image2.height();

    size = ui->qlImgLabel2->size();
    image2 = image2.scaled(size, Qt::KeepAspectRatio, Qt::SmoothTransformation);

    painter = new QPainter(&image2);
    painter->setPen(Qt::white);
    painter->setFont(QFont("Arial", 10));
    painter->drawText(image2.rect(), Qt::AlignTop | Qt::AlignLeft, QString::fromStdString(dim.str()));
    delete painter;

//  QLabel* qlImgLabel2 = new QLabel;
//  qlImgLabel2->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
//  qlImgLabel2->setScaledContents(false);
//  ui->saViewImg2->setWidget(qlImgLabel2);
    ui->saViewImg2->setBackgroundRole(QPalette::Dark);

    ui->qlImgLabel2->setPixmap(QPixmap::fromImage(image2));

    // Display image2 name
    name_size.str("");
    name_size << fileName.toStdString() << "  " << std::setprecision(1) 
              << std::fixed << QFileInfo(fileName).size()/1024.0 << "K";
    ui->leImgInfo2->setText(name_size.str().c_str());

    // Enable "Delete2" button
    if ( ! ui->pbDelImg2->isEnabled() )
        ui->pbDelImg2->setEnabled(true);
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
    ui->twDupsTable->clearContents();
    ui->twDupsTable->setRowCount(0);

    ui->qlImgLabel1->clear();
    ui->leImgInfo1->clear();
    ui->pbDelImg1->setEnabled(false);

    ui->qlImgLabel2->clear();
    ui->leImgInfo2->clear();
    ui->pbDelImg2->setEnabled(false);

    ui->progressBar->setValue(0);
}


void ImgMatchUI::on_pbDelImg1_clicked()
{
    
}


void ImgMatchUI::on_pbDelImg2_clicked()
{
    
}
