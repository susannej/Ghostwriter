#include "GhostwriterPlugin.h"
#include "GhostwriterFilter.h"
#include "PluginHub.h"
#include "PluginDependency.h"
#include "SampleGroupSettings.h"

#include <QDebug>


extern "C" BIBBLE_API BaseB5Plugin *b5plugin() { return new GhostwriterPlugin; }


bool GhostwriterPlugin::init(PluginHub *hub, int id, int groupId, const QString &)
{
	Q_UNUSED( groupId );
	
	m_hub = hub;
	m_id = id;
	m_groupId = groupId;
	return true;
}

bool GhostwriterPlugin::registerFilters()
{
	GhostwriterFilter *mirror = new GhostwriterFilter(m_hub, m_groupId);
	if (m_hub->addFilter(mirror, PluginHub::Tail, QString())) {
		return true;
	}
	qDebug() << "Failed to load the mirror filter";
	return false;
}

bool GhostwriterPlugin::registerOptions()
{
	m_hub->addBoolOption(SampleGroup::kEnableInvert, "invertOn", "Sample Invert", "Inverts the image", "Invert5.1_Filter", false, false, 0);
	return true;
}

bool GhostwriterPlugin::finish()
{
	return true;
}

PluginDependency *GhostwriterPlugin::createDependency(const QString &name)
{
	Q_UNUSED(name);
	return NULL;
}

QList<QString> GhostwriterPlugin::toolFiles()
{
	return QList<QString>();
}

QList<QWidget*> GhostwriterPlugin::toolWidgets()
{
	return QList<QWidget*>();
}

void GhostwriterPlugin::toolWidgetCreated(QWidget *uiWidget)
{
	Q_UNUSED( uiWidget );
}

