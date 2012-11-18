#include "GhostwriterPlugin.h"
#include "GhostwriterFilter.h"
#include "PluginHub.h"
#include "PluginDependency.h"
#include "PluginOptionList.h"
#include "PluginImageSettings.h"


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
	b &= m_hub->addIntOption(GHOSTWRITER_HEIGHT, "height", "Height of the titlebar", GHOSTWRITER_GROUP, "Height of the titlebar in % of the image height", 15, 15, 0);
	b &= m_hub->addIntOption(GHOSTWRITER_LUCENT, "lucent", "transparency of the titlebar", GHOSTWRITER_GROUP, "Transparency of the titlebar (0 = opaque / 100 = transparent)", 0, 0, 0);
	//b &= m_hub->m_addStringOption(GHOSTWRITER_LOGOFILENAME, "logofilename1", "Filename of the png-file within the titlebar", GHOSTWRITER_GROUP, "Filename of the png-file for the titlebar", findFirstLogo(), OPTION_RESTART);
	b &= m_hub->addIntOption(GHOSTWRITER_LOGOSIZE, "logosize", "size of the png-file within the titlebar", GHOSTWRITER_GROUP, "size of the logo within the titlebar in % of the titlebar-height", 50, 50, 0);
	b &= m_hub->addIntOption(GHOSTWRITER_LOGOORIENTATION, "orientation", "orientation of the png-file within the titlebar", GHOSTWRITER_GROUP, "orientation (left/right) of the logo within the titlebar", 0, 0, 0);
	b &= m_hub->addIntOption(GHOSTWRITER_LOGOTRANS, "logotrans", "orientation of the png-file within the titlebar", GHOSTWRITER_GROUP, "additional adjustement for the logo transparency. (0 = original png transparency / 100 totaly transparence)", 0, 0, 0);
	//b &= m_hub->addBoolOption(GHOSTWRITER_FIT, "fit", "fit the logo into the crop area", GHOSTWRITER_GROUP, "fit the logo again into the cropped area", false, false, 0);

	// Logo2 Tab
	b &= m_hub->addBoolOption(GHOSTWRITER_LOGO2ENABLED, "logo2", "Enable 2nd Logo", GHOSTWRITER_GROUP, "Enable 2nd Logo", false, false, 0);
	//b &= m_hub->m_addStringOption(GHOSTWRITER_LOGOFILENAME2, "logofilename2", "Name of the 2nd logo", GHOSTWRITER_GROUP, "Name of the 2nd logo" , findFirstLogo(), OPTION_RESTART);
	b &= m_hub->addIntOption(GHOSTWRITER_LOGOSIZE2, "logosize2", "size of the 2nd logo", GHOSTWRITER_GROUP, "Size of the 2nd logo in % of the image-height" , 10, 10, 0);
	b &= m_hub->addIntOption(GHOSTWRITER_XPOS2, "xpos2", "x-position of the 2nd logo", GHOSTWRITER_GROUP, "x-position of the 2nd logo" , 0, 0, 0);
	b &= m_hub->addIntOption(GHOSTWRITER_YPOS2, "ypos2", "y-position of the 2nd logo", GHOSTWRITER_GROUP, "y-position of the 2nd logo" , 0, 0, 0);
	b &= m_hub->addIntOption(GHOSTWRITER_LOGOTRANS2, "logotrans2", "transparency of the 2nd logo", GHOSTWRITER_GROUP, "transparency of the 2nd logo" , 0, 0, 0);

	// Config Tab
	//baf->m_addStringOption( bibbleData->m_plugin, GHOSTWRITER_LOGODIR, "logodir", "Directory Name for saved logofiles", GHOSTWRITER_GROUP, "Directory Name for saved logofiles", readConfigFile(), OPTION_DONT_RESTART | OPTION_DOESNOTPERSIST);

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

	//	connect the "Fit" button to a slot
	if ( QAbstractButton * pBtn = uiWidget->findChild< QAbstractButton * >( GHOSTWRITER_REFRESH_BNNAME ) )
	{
		connect( pBtn, SIGNAL( clicked() ), SLOT( fitIntoCrop() ) );
	}

	//	connect the "Save" button to a slot
	if ( QAbstractButton * pBtn = uiWidget->findChild< QAbstractButton * >( GHOSTWRITER_SAVE_BNNAME ) )
	{
		connect( pBtn, SIGNAL( clicked() ), SLOT( save() ) );
	}
	
	m_pWindow = uiWidget->window();

}

//-----------------------------------------------------------------------------
void GhostwriterPlugin::showAbout() const
{
	QMessageBox::about( m_pWindow, tr( "AfterShot Pro Ghostwriter Plugin" ), 
					    tr( "Ghostwriter Bibble5 Plugin (V 2.0)\n(C) 2012 Susanne Jaeckel - Neuss, Germany\n\ncontact me at susanne_j@gmx.de" ) );
}

//-----------------------------------------------------------------------------
void GhostwriterPlugin::fitIntoCrop() const
{
	QMessageBox::about( m_pWindow, tr( "AfterShot Pro Ghostwriter Plugin" ), 
					    tr( "Ghostwriter Bibble5 Plugin (V 2.0)\n(C) 2012 Susanne Jaeckel - Neuss, Germany\n\ncontact me at susanne_j@gmx.de" ) );
}

//-----------------------------------------------------------------------------
void GhostwriterPlugin::save() const
{
	QMessageBox::about( m_pWindow, tr( "AfterShot Pro Ghostwriter Plugin" ), 
					    tr( "Ghostwriter Bibble5 Plugin (V 2.0)\n(C) 2012 Susanne Jaeckel - Neuss, Germany\n\ncontact me at susanne_j@gmx.de" ) );
}

//-----------------------------------------------------------------------------
void GhostwriterPlugin::handleControlChange( const QString &optionName, int groupId, int layer, const PluginImageSettings &options, 
											   const PluginOptionList &, PluginOptionList &changes )
{
	//	some changes do not have options - check for NULL
	if ( options.options( layer ) == NULL )
		return;
	
	//	ignore changes that aren't for us
	if ( groupId != m_groupId )
		return;
	
	/*
	float R, G, B;
	float H, S, V;
	int		hue;
	int		rgb;
	QRgb	qrgb;
	bool	bOk = true;
	
	//	color picker 1 changed, update Hue 1
	if ( optionName == "bbwcolorA" )
	{
		qrgb = changes.getInt( BW_C1PICKER, m_groupId, bOk );
		
		if ( bOk )
		{
			R = qRed( qrgb );
			G = qGreen( qrgb );
			B = qBlue( qrgb );

			BlackAndWhiteFilter::BBW_RGBtoHSV( R, G, B, &H, &S, &V );
			
			changes.setInt( BW_HUE, m_groupId, qRound( H ) );
		}
	}
	
	//	color picker 2 changed, update Hue 2
	else if ( optionName == "bbwcolorB" )
	{
		qrgb = changes.getInt( BW_C2PICKER, m_groupId, bOk );
		
		if ( bOk )
		{
			R = qRed( qrgb );
			G = qGreen( qrgb );
			B = qBlue( qrgb );
			
			BlackAndWhiteFilter::BBW_RGBtoHSV( R, G, B, &H, &S, &V );
			
			changes.setInt( BW_HUE2, m_groupId, qRound( H ) );
		}
	}
	
	//	Hue 1 changed, update color picker 1
	else if ( optionName == "bbwhueA" )
	{
		S = 1.f;
		V = 255.f;
		hue = changes.getInt( BW_HUE, m_groupId, bOk );
		
		if ( bOk )
		{
			BlackAndWhiteFilter::BBW_HSVtoRGB( &R, &G, &B, hue, S, V );
			rgb = qRgb( R, G, B );
			changes.setInt( BW_C1PICKER, m_groupId, rgb );
		}
	}
	
	//	Hue 2 changed, update color picker 2
	else if ( optionName == "bbwhueB" )
	{
		S = 1.f;
		V = 255.f;
		hue = changes.getInt( BW_HUE2, m_groupId, bOk );
		
		if ( bOk )
		{
			BlackAndWhiteFilter::BBW_HSVtoRGB( &R, &G, &B, hue, S, V );
			rgb = qRgb( R, G, B );
			changes.setInt( BW_C2PICKER, m_groupId, rgb );
		}
	}
	*/
}
