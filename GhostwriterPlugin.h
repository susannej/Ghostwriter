#pragma once

#include "B5Plugin.h"
#include <QString>

class GhostwriterPlugin : public B5Plugin
{
public:
	GhostwriterPlugin() { ; }
	virtual int priority() { return 100; }
	virtual QString name() { return QString("Invert5.1"); }
	virtual QString group() { return QString("sampleGroup"); }
	virtual bool init(PluginHub *hub, int id, int groupId, const QString &bundlePath);
	virtual bool registerFilters();
	virtual bool registerOptions();
	virtual bool finish();
	virtual PluginDependency *createDependency(const QString &name);
    virtual QList<QString> toolFiles();
    virtual QList<QWidget*> toolWidgets();
    virtual void toolWidgetCreated(QWidget *uiWidget);
private:
	PluginHub *m_hub;
	int m_id;
	int m_groupId;
};
