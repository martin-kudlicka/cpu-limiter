#ifndef RULEDIALOG_H
#define RULEDIALOG_H

#include "ui_ruledialog.h"
#include "ruleoptions.h"
#include <MkWidgets/MWidgetSettings>

class RuleDialog : public QDialog
{
  Q_OBJECT

  public:
             RuleDialog(QWidget *parent);
             RuleDialog(const MUuidPtr &id, QWidget *parent);
    virtual ~RuleDialog() Q_DECL_OVERRIDE Q_DECL_EQ_DEFAULT;

  private:
    RuleOptions     _options;
    Ui::RuleDialog  _ui;
    MWidgetSettings _widgetSettings;

    void setupSettings ();
    void updateOkButton(bool preEnabled = true) const;

    virtual void accept() Q_DECL_OVERRIDE;

  private slots:
    void on_name_textChanged(const QString &text) const;
};

#endif