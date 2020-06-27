#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "my_qt_common.h"
#include "runcrack.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void slot_clear_chkLabel();
    void slot_clear_chkLabel(int);
    void slot_init_and_print_examples(const QString &);
    void slot_switch_to_force_crack_or_test();

    void slot_done_crack_force(qint8 is_success);
    void slot_realtime_crack_force_info(QString info);

private slots:
    void on_chkBtn_clicked();

signals:
    void sig_crack_force(qint16 minlen, qint16 maxlen);

private:
    Ui::MainWindow *ui;
    int m_algoid;
    RunCrack * m_run_crack;
};

#endif // MAINWINDOW_H
