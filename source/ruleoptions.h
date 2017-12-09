#ifndef RULEOPTIONS_H
#define RULEOPTIONS_H

#include <MkCore/MSettings>
#include <MkCore/MUuidPtr>

class RuleOptions : public MSettings
{
  public:
    enum class SelectedProcesses
    {
      Condition,
      Target
    };

    class Property
    {
      public:
        static Q_DECL_CONSTEXPR QString Enabled;
        static Q_DECL_CONSTEXPR QString Name;

        static Q_DECL_CONSTEXPR QString Condition_SelectedProcesses;
        static Q_DECL_CONSTEXPR QString Condition_Status;

        static Q_DECL_CONSTEXPR QString Target_Action;
        static Q_DECL_CONSTEXPR QString Target_CPULimit;
        static Q_DECL_CONSTEXPR QString Target_SelectedProcesses;
    };

             RuleOptions(MUuidPtr &&id);
             RuleOptions(const MUuidPtr &id);
    virtual ~RuleOptions() Q_DECL_OVERRIDE Q_DECL_EQ_DEFAULT;

    const MUuidPtr &id                   () const;
          QString   name                 () const;
          void      setSelectedProcess   (SelectedProcesses type, quintptr index, const QString &name);
          QString   selectedProcess      (SelectedProcesses type, quintptr index);
          quintptr  selectedProcessesSize(SelectedProcesses type);

  private:
    MUuidPtr _id;
};

#endif