#ifndef RULEOPTIONS_H
#define RULEOPTIONS_H

#include <MkCore/MSettings>
#include <MkCore/MUuidPtr>

class RuleOptions : public MSettings
{
  public:
    enum class Action
    {
      SetCpuRate,
      Suspend
    };
    enum class InternetConnection
    {
      Disconnected,
      Connected
    };
    enum class Section
    {
      Condition,
      Target
    };
    enum class State
    {
      Anyhow,
      Foreground,
      Background
    };
    enum class Status
    {
      Running,
      NotRunning
    };

    struct Property
    {
      static const QString Enabled;
      static const QString Name;

      static const QString Condition_InternetConnection;
      static const QString Condition_InternetConnectionStatus;
      static const QString Condition_SelectedProcesses;
      static const QString Condition_State;
      static const QString Condition_Status;

      static const QString Target_Action;
      static const QString Target_ApplyDelay;
      static const QString Target_ApplyDelayValue;
      static const QString Target_CPULimit;
      static const QString Target_SelectedProcesses;
    };

             RuleOptions(const MUuidPtr &id);
    virtual ~RuleOptions() Q_DECL_OVERRIDE Q_DECL_EQ_DEFAULT;

          Action             action                  () const;
          bool               applyDelay              () const;
          quintptr           applyDelayValue         () const;
          quintptr           cpuLimit                () const;
          bool               enabled                 () const;
    const MUuidPtr          &id                      () const;
          bool               internetConnectionCheck () const;
          InternetConnection internetConnectionStatus() const;
          QString            name                    () const;
          QString            selectedProcess         (Section section, quintptr index);
          QStringList        selectedProcesses       (Section section);
          quintptr           selectedProcessesSize   (Section section);
          void               setEnabled              (bool enabled);
          State              state                   () const;
          Status             status                  () const;

  private:
    MUuidPtr _id;
};

#endif