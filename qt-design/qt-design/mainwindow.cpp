#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //
    ui->switchForceRBtn->setText("crack test.");
    //
    this->setWindowTitle("Hash Crack for test Tool v1.0");
    ui->chkBtn->setDefault(true);
    //
    ui->chkLabel->setText("waiting for this crack test... ...");
    //
    ui->inPwdEdit->setStyleSheet("color:gray");
    ui->inPwdEdit->setEnabled(false);
    //
    void (MainWindow::*slot_func1)() = &MainWindow::slot_clear_chkLabel;
    this->connect(ui->inTextEdit, &QTextEdit::textChanged, this, slot_func1);
    this->connect(ui->inPwdEdit, &QLineEdit::textChanged, this, slot_func1);
    void (MainWindow::*slot_func2)(int) = &MainWindow::slot_clear_chkLabel;
    void (QComboBox::*sig_func)(int) = &QComboBox::currentIndexChanged;
    this->connect(ui->typeComboBox, sig_func, this, slot_func2);
    this->connect(ui->typeComboBox, &QComboBox::currentTextChanged, this, &MainWindow::slot_init_and_print_examples);
    //
    this->connect(ui->switchForceRBtn, &QRadioButton::clicked, this, &MainWindow::slot_switch_to_force_crack_or_test);
    //
    m_run_crack = new RunCrack();
    this->connect(m_run_crack, &RunCrack::sig_done_crack_force, this, &MainWindow::slot_done_crack_force);
    this->connect(m_run_crack, &RunCrack::sig_realtime_crack_force_info, this, &MainWindow::slot_realtime_crack_force_info);
    this->connect(this, &MainWindow::sig_crack_force, m_run_crack, &RunCrack::slot_crack_force, Qt::QueuedConnection);
    //
    ui->deviceComboBox->addItem(" cpu: single-thread");
    ui->deviceComboBox->addItem(" cpu:  multi-thread");
    ui->deviceComboBox->addItem(" cpu: opencl");
    ui->deviceComboBox->addItem(" gpu: opencl");
    ui->deviceComboBox->addItem(" gpu:   cuda");
    //
    ui->typeComboBox->addItem("    0:MD5");
    ui->typeComboBox->addItem("   10:md5($pass.$salt)");
    ui->typeComboBox->addItem("   20:md5($salt.$pass)");
    ui->typeComboBox->addItem("   50:HMAC-MD5 (key = $pass)");
    ui->typeComboBox->addItem("  111:nsldaps, SSHA-1(Base64), Netscape LDAP SSHA");
    ui->typeComboBox->addItem("  150:HMAC-SHA1 (key = $pass)");
    ui->typeComboBox->addItem("  200:mysql323");
    ui->typeComboBox->addItem("  300:mysql4.1/mysql5");
    ui->typeComboBox->addItem("  400:phpass,WordPress(MD5),Joomla(MD5)/phpass,phpBB3(MD5)");
    ui->typeComboBox->addItem("  500:md5crypt, MD5(Unix), Cisco-IOS $1$(MD5)");
    ui->typeComboBox->addItem("  501:Juniper IVE");
    ui->typeComboBox->addItem(" 1100:Domain Cached Credentials (DCC), MS Cache");
    ui->typeComboBox->addItem(" 1400:SHA2-256");
    ui->typeComboBox->addItem(" 1450:HMAC-SHA256 (key = $pass)");
    ui->typeComboBox->addItem(" 1500:descrypt, DES(Unix), Traditional DES");
    ui->typeComboBox->addItem(" 1600:Apache $apr1$ MD5, md5apr1, MD5(APR) 2");
    ui->typeComboBox->addItem(" 1711:SSHA-512(Base64), LDAP {SSHA512}");
    ui->typeComboBox->addItem(" 1731:mssql (2012, 2014)");
    ui->typeComboBox->addItem(" 1800:sha512crypt $6$, SHA512(Unix)");
    ui->typeComboBox->addItem(" 2100:DCC2 MSCahe2");
    ui->typeComboBox->addItem(" 9400:office 2007");
    ui->typeComboBox->addItem(" 9500:office 2010");
    ui->typeComboBox->addItem(" 9600:office 2013-2016");
    ui->typeComboBox->addItem(" 9700:office 2003$1");
    ui->typeComboBox->addItem("12900:Android FDE (Samsung DEK)");
}

MainWindow::~MainWindow()
{
    delete m_run_crack;
    delete ui;
}

void MainWindow::slot_clear_chkLabel()
{
    ui->chkLabel->setText("waiting for this crack test... ...");
}
void MainWindow::slot_clear_chkLabel(int)
{
    ui->chkLabel->setText("waiting for this crack test... ...");
}

void MainWindow::slot_init_and_print_examples(const QString & text)
{
    // 1. get the selected algo type:
    QString qstr_algo_type = text.trimmed();
    QStringList qstr_algo_type_list = qstr_algo_type.split(":");
    QString qstr_algo_id = qstr_algo_type_list.at(0);
    qstr_algo_type.clear();
    qstr_algo_type_list.clear();
    int algo_id = qstr_algo_id.toInt();
    qstr_algo_id.clear();
    if (algo_id < 0)
    {
        QMessageBox::warning(this, "ERROR", "The Algo ID must be large than 0!");
        return;
    }
    m_algoid = algo_id;
    // 2. disable crack btn:
    ui->chkBtn->setEnabled(false);
    // 3. emit signal of init crack:
    m_run_crack->crack_init(algo_id);
    //
    QString example_pwd = m_run_crack->get_example_pwd();
    QString example_hash = m_run_crack->get_example_hash();
    if (!example_pwd.isEmpty() && !example_hash.isEmpty())
    {
        // 1. print crack's example pair{pwd, hash}:
        ui->inPwdEdit->setStyleSheet("color:black");
        ui->inTextEdit->setTextColor(QColor("black"));
        ui->inPwdEdit->setText(example_pwd);
        ui->inTextEdit->setText(example_hash);
        // 2. enable crack btn:
        ui->chkBtn->setEnabled(true);
        // if
        if (!ui->switchForceRBtn->isChecked())
        {
            ui->inPwdEdit->setStyleSheet("color:gray");
        }
    }
    else
    {
        //
        ui->inPwdEdit->setStyleSheet("color:red");
        ui->inTextEdit->setTextColor(QColor("red"));
        ui->inPwdEdit->setText("no support algo; no example pwd!");
        ui->inTextEdit->setText("no support algo; no example hash!");
        //
        ui->chkBtn->setEnabled(false);
    }
}

void MainWindow::slot_switch_to_force_crack_or_test()
{
    if (ui->switchForceRBtn->isChecked())
    {
        ui->chkBtn->setText("Crack for Test (check the password is or not corresponding the Hash)");
        ui->switchForceRBtn->setText("force crack");
        ui->inPwdEdit->setStyleSheet("color:black");
        ui->inPwdEdit->setEnabled(true);
        ui->minLenEdit->setEnabled(false);
        ui->maxLenEdit->setEnabled(false);
        ui->deviceComboBox->setCurrentIndex(0);
        ui->deviceComboBox->setEnabled(false);
    }
    else
    {
        ui->chkBtn->setText("force crack");
        ui->switchForceRBtn->setText("crack test.");
        ui->inPwdEdit->setStyleSheet("color:gray");
        ui->inPwdEdit->setEnabled(false);
        ui->minLenEdit->setEnabled(true);
        ui->maxLenEdit->setEnabled(true);
        ui->deviceComboBox->setEnabled(true);
    }
}

void MainWindow::on_chkBtn_clicked()
{
    ui->chkLabel->setText("is going crack ... ...");
    // 1. reinit crack data:
    m_run_crack->crack_reinit();
    // 2. in hash:
    QString qstr_hash = ui->inTextEdit->toPlainText().trimmed();
    QByteArray ba_hash = qstr_hash.toLatin1();
    char * in_hash = (char*)malloc( ba_hash.size()+128 );
    memset(in_hash, 0x00, ba_hash.size()+128);
    strcpy(in_hash, ba_hash.data());
    qstr_hash.clear();
    ba_hash.clear();
    // 3. parse hash:
    STATE_FLAG parse_flag = m_run_crack->parse_hash(in_hash, strlen(in_hash));
    if (parse_flag != SUCCESS_RET)
    {
        free(in_hash);
        in_hash = NULL;
        m_run_crack->crack_delete();
        QMessageBox::warning(this, "ERROR", QString("This Hash %1 cannot parse!").arg(in_hash));
        return;
    }
    free(in_hash);
    in_hash = NULL;
    // 4. crack for test:
    // or
    //    crack force:
    if (ui->inPwdEdit->isEnabled())
    { //// crack for test:
        // 4.1. in password:
        QString qstr_pwd = ui->inPwdEdit->text();   //.trimmed();  hold origin input!
        QByteArray ba_pwd = qstr_pwd.toLatin1();
        char * in_pwd = (char*)malloc( ba_pwd.size()+128 );
        memset(in_pwd, 0x00, ba_pwd.size()+128);
        strcpy(in_pwd, ba_pwd.data());
        qstr_pwd.clear();
        ba_pwd.clear();
        // 4.2. crack for test:
        STATE_FLAG crack_test_flag = m_run_crack->crack_for_test(in_pwd, strlen(in_pwd));
        m_run_crack->free_parse_hash();
        free(in_pwd);
        in_pwd = NULL;
        m_run_crack->crack_delete();
        // 4.3. check test: password is or not corresponding the hash
        if (crack_test_flag == SUCCESS_RET)
        {     //  OK
            ui->chkLabel->setText("Finished this crack test: Success!");
        }
        else
        {     //  Fail
            ui->chkLabel->setText("Finished this crack test:    Fail!");
        }
    }
    else
    { //// crack force:
        QString qstr_minlen = ui->minLenEdit->text().trimmed();
        int minlen = qstr_minlen.toInt();
        minlen = minlen<=0?1:minlen;
        QString qstr_maxlen = ui->maxLenEdit->text().trimmed();
        int maxlen = qstr_maxlen.toInt();
        if (maxlen <= 0)
        {
            m_run_crack->free_parse_hash();
            m_run_crack->crack_delete();
            QMessageBox::warning(this, "ERROR", QString("force-crack max len must > 0 !"));
            return;
        }
        //
        ui->inTextEdit->setEnabled(false);
        ui->switchForceRBtn->setEnabled(false);
        ui->typeComboBox->setEnabled(false);
        ui->chkBtn->setEnabled(false);
        ui->minLenEdit->setEnabled(false);
        ui->maxLenEdit->setEnabled(false);
        //
        emit sig_crack_force(minlen, maxlen);
    }
}

void MainWindow::slot_done_crack_force(qint8 is_success)
{
    //
    ui->inTextEdit->setEnabled(true);
    ui->switchForceRBtn->setEnabled(true);
    ui->typeComboBox->setEnabled(true);
    ui->chkBtn->setEnabled(true);
    ui->minLenEdit->setEnabled(true);
    ui->maxLenEdit->setEnabled(true);
    //
    if (is_success)
    {
        ui->chkLabel->setText(QString("Success force-cracked right password(%1)").arg(m_run_crack->get_correct_pwd()));
    }
    else
    {
        ui->chkLabel->setText(QString("Not force-cracked right password!"));
    }
    m_run_crack->free_parse_hash();
    m_run_crack->crack_delete();
}

void MainWindow::slot_realtime_crack_force_info(QString info)
{
    ui->resultEdit->clear();
    ui->resultEdit->append(info);
}

