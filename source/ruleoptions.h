#ifndef RULEOPTIONS_H
#define RULEOPTIONS_H

#include <MkCore/MSettings>

class RuleOptions : public MSettings
{
  public:
    class Property
    {
      public:
        static const QString Enabled;
        static const QString Name;
    };
};

#endif