#ifndef RULEOPTIONS_H
#define RULEOPTIONS_H

#include <MkCore/MSettings>
#include <MkCore/MUuidPtr>

class RuleOptions : public MSettings
{
  public:
    class Property
    {
      public:
        static const QString Condition_Status;
        static const QString Enabled;
        static const QString Name;
    };

    virtual ~RuleOptions() Q_DECL_OVERRIDE Q_DECL_EQ_DEFAULT;
};

#endif