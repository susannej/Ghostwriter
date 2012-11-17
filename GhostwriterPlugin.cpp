#include "GhostwriterPlugin.h"
#include "GhostwriterFilter.h"
#include "PluginHub.h"
#include "PluginDependency.h"

#include <QDebug>
#include <QComboBox>
#include <QAbstractButton>
#include <QMessageBox>


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
	qDebug() << "Failed to load the Ghostwriter filter";
	return false;
}

bool GhostwriterPlugin::registerOptions()
{
	bool b = true;

	b &= m_hub->addBoolOption(GHOSTWRITER_ENABLED, "gwenabled", "Ghostwriter", "enable Ghostwriter plugin", "Ghostwriter_Filter", false, false, 0);

	if (b == false)
		qDebug() << "GhostwriterPlugin: Unable to register options.";

	return b;

	/*
	baf->m_addBoolOption( bibbleData->m_plugin, GHOSTWRITER_ENABLED, "gwenabled", "Ghostwriter", GHOSTWRITER_GROUP, "enable Ghostwriter plugin", false, OPTION_RESTART);
	baf->m_addIntOption( bibbleData->m_plugin, GHOSTWRITER_HEIGHT, "height", "Height of the titlebar", GHOSTWRITER_GROUP, "Height of the titlebar in % of the image height", 15, OPTION_RESTART);
	baf->m_addIntOption( bibbleData->m_plugin, GHOSTWRITER_LUCENT, "lucent", "transparency of the titlebar", GHOSTWRITER_GROUP, "Transparency of the titlebar (0 = opaque / 100 = transparent)", 0, OPTION_RESTART);
	baf->m_addStringOption( bibbleData->m_plugin, GHOSTWRITER_LOGOFILENAME, "logofilename1", "Filename of the png-file within the titlebar", GHOSTWRITER_GROUP, "Filename of the png-file for the titlebar", findFirstLogo(), OPTION_RESTART);
	baf->m_addIntOption( bibbleData->m_plugin, GHOSTWRITER_LOGOSIZE, "logosize", "size of the png-file within the titlebar", GHOSTWRITER_GROUP, "size of the logo within the titlebar in % of the titlebar-height", 50, OPTION_RESTART);
	baf->m_addIntOption( bibbleData->m_plugin, GHOSTWRITER_LOGOORIENTATION, "orientation", "orientation of the png-file within the titlebar", GHOSTWRITER_GROUP, "orientation (left/right) of the logo within the titlebar", 0, OPTION_RESTART);
	baf->m_addIntOption( bibbleData->m_plugin, GHOSTWRITER_LOGOTRANS, "logotrans", "orientation of the png-file within the titlebar", GHOSTWRITER_GROUP, "additional adjustement for the logo transparency. (0 = original png transparency / 100 totaly transparence)", 0, OPTION_RESTART);
	baf->m_addBoolOption( bibbleData->m_plugin, GHOSTWRITER_FIT, "fit", "fit the logo into the crop area", GHOSTWRITER_GROUP, "fit the logo again into the cropped area", false, OPTION_RESTART | OPTION_DOESNOTPERSIST);

	// Logo2 Tab
	baf->m_addBoolOption( bibbleData->m_plugin, GHOSTWRITER_LOGO2ENABLED, "logo2", "Enable 2nd Logo", GHOSTWRITER_GROUP, "Enable 2nd Logo", false, OPTION_RESTART);
	baf->m_addStringOption( bibbleData->m_plugin, GHOSTWRITER_LOGOFILENAME2, "logofilename2", "Name of the 2nd logo", GHOSTWRITER_GROUP, "Name of the 2nd logo" , findFirstLogo(), OPTION_RESTART);
	baf->m_addIntOption( bibbleData->m_plugin, GHOSTWRITER_LOGOSIZE2, "logosize2", "size of the 2nd logo", GHOSTWRITER_GROUP, "Size of the 2nd logo in % of the image-height" , 10, OPTION_RESTART);
	baf->m_addIntOption( bibbleData->m_plugin, GHOSTWRITER_XPOS2, "xpos2", "x-position of the 2nd logo", GHOSTWRITER_GROUP, "x-position of the 2nd logo" , 0, OPTION_RESTART);
	baf->m_addIntOption( bibbleData->m_plugin, GHOSTWRITER_YPOS2, "ypos2", "y-position of the 2nd logo", GHOSTWRITER_GROUP, "y-position of the 2nd logo" , 0, OPTION_RESTART);
	baf->m_addIntOption( bibbleData->m_plugin, GHOSTWRITER_LOGOTRANS2, "logotrans2", "transparency of the 2nd logo", GHOSTWRITER_GROUP, "transparency of the 2nd logo" , 0, OPTION_RESTART);

	// Config Tab
	//baf->m_addStringOption( bibbleData->m_plugin, GHOSTWRITER_LOGODIR, "logodir", "Directory Name for saved logofiles", GHOSTWRITER_GROUP, "Directory Name for saved logofiles", readConfigFile(), OPTION_DONT_RESTART | OPTION_DOESNOTPERSIST);
	*/
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

	//	connect the "?" button to a slot to show info about the plugin
	if ( QAbstractButton * pBtn = uiWidget->findChild< QAbstractButton * >( GHOSTWRITER_ABOUT_BNNAME ) )
	{
		connect( pBtn, SIGNAL( clicked() ), SLOT( showAbout() ) );
	}
	
	m_pWindow = uiWidget->window();

}

//-----------------------------------------------------------------------------
void GhostwriterPlugin::showAbout() const
{
	QMessageBox::about( m_pWindow, tr( "AfterShot Pro Ghostwriter Plugin" ), 
					    tr( "Ghostwriter Bibble5 Plugin (V 2.0)\n(C) 2012 Susanne Jaeckel - Neuss, Germany\n\ncontact me at susanne_j@gmx.de" ) );
}

