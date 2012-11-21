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

#ifdef WIN32
#include "dirent.h"
#else
#include <dirent.h>
#endif



extern "C" BIBBLE_API BaseB5Plugin *b5plugin() { return new GhostwriterPlugin; }


bool GhostwriterPlugin::init(PluginHub *hub, int id, int groupId, const QString &)
{
	//Q_UNUSED( groupId );
	
	m_hub = hub;
	m_id = id;
	m_groupId = groupId;
	m_logoDirName = NULL;

	readConfigFile();
	return true;
}

bool GhostwriterPlugin::registerFilters()
{
	GhostwriterFilter *pFilter = new GhostwriterFilter(m_hub, m_groupId);
	if (m_hub->addFilter(pFilter, PluginHub::Tail, QString())) {
		return true;
	}
	qDebug() << "Failed to load the Ghostwriter filter";
	return false;
}

bool GhostwriterPlugin::registerOptions()
{
	bool b = true;

	b &= m_hub->addBoolOption(GHOSTWRITER_ENABLED, "gwenabled", "Ghostwriter", "enable Ghostwriter plugin", GHOSTWRITER_FILTER, false, false, 0);
	b &= m_hub->addIntOption(GHOSTWRITER_HEIGHT, "height", "Height of the titlebar", "Height of the titlebar in % of the image height", GHOSTWRITER_FILTER, 15, 15, 0);
	b &= m_hub->addIntOption(GHOSTWRITER_LUCENT, "lucent", "transparency of the titlebar", "Transparency of the titlebar (0 = opaque / 100 = transparent)", GHOSTWRITER_FILTER, 0, 0, 0);
	//b &= m_hub->m_addStringOption(GHOSTWRITER_LOGOFILENAME, "logofilename1", "Filename of the png-file within the titlebar", GHOSTWRITER_GROUP, "Filename of the png-file for the titlebar", findFirstLogo(), OPTION_RESTART);
	b &= m_hub->addIntOption(GHOSTWRITER_LOGOSIZE, "logosize", "size of the png-file within the titlebar", "size of the logo within the titlebar in % of the titlebar-height", GHOSTWRITER_FILTER, 50, 50, 0);
	b &= m_hub->addIntOption(GHOSTWRITER_LOGOORIENTATION, "orientation", "orientation of the png-file within the titlebar", "orientation (left/right) of the logo within the titlebar", GHOSTWRITER_FILTER, 0, 0, 0);
	b &= m_hub->addIntOption(GHOSTWRITER_LOGOTRANS, "logotrans", "orientation of the png-file within the titlebar", "additional adjustement for the logo transparency. (0 = original png transparency / 100 totaly transparence)", GHOSTWRITER_FILTER, 0, 0, 0);
	b &= m_hub->addBoolOption(GHOSTWRITER_FIT, "fit", "fit the logo into the crop area", "fit the logo again into the cropped area", GHOSTWRITER_FILTER, false, false, 0);

	// Logo2 Tab
	b &= m_hub->addBoolOption(GHOSTWRITER_LOGO2ENABLED, "logo2", "Enable 2nd Logo", "Enable 2nd Logo", GHOSTWRITER_FILTER, false, false, 0);
	//b &= m_hub->m_addStringOption(GHOSTWRITER_LOGOFILENAME2, "logofilename2", "Name of the 2nd logo", GHOSTWRITER_GROUP, "Name of the 2nd logo" , findFirstLogo(), OPTION_RESTART);
	b &= m_hub->addIntOption(GHOSTWRITER_LOGOSIZE2, "logosize2", "size of the 2nd logo", "Size of the 2nd logo in % of the image-height" , GHOSTWRITER_FILTER, 10, 10, 0);
	b &= m_hub->addIntOption(GHOSTWRITER_XPOS2, "xpos2", "x-position of the 2nd logo", "x-position of the 2nd logo" , GHOSTWRITER_FILTER, 0, 0, 0);
	b &= m_hub->addIntOption(GHOSTWRITER_YPOS2, "ypos2", "y-position of the 2nd logo", "y-position of the 2nd logo" , GHOSTWRITER_FILTER, 0, 0, 0);
	b &= m_hub->addIntOption(GHOSTWRITER_LOGOTRANS2, "logotrans2", "transparency of the 2nd logo", "transparency of the 2nd logo" , GHOSTWRITER_FILTER, 0, 0, 0);

	// Config Tab
	//baf->m_addStringOption( bibbleData->m_plugin, GHOSTWRITER_LOGODIR, "logodir", "Directory Name for saved logofiles", GHOSTWRITER_GROUP, "Directory Name for saved logofiles", readConfigFile(), OPTION_DONT_RESTART | OPTION_DOESNOTPERSIST);

	if (b == false)
		qDebug() << "GhostwriterPlugin: Unable to register options.";

	return b;
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
	//	Need to initialize the comobo boxes with the logo filenames
	//	find the combo box, it's named bbwmode_combo

	if ( QComboBox * pCombo = uiWidget->findChild< QComboBox * >( "logofilename1_combo" ) )
	{
		pCombo->clear();

		QList<QString>logoNames = readLogoNames();
		for (int i = 0; i < logoNames.size(); i++) {
			pCombo->addItem( logoNames[i], i );
		}
		
		pCombo->setMaxVisibleItems( pCombo->count() );
	}

	if ( QComboBox * pCombo = uiWidget->findChild< QComboBox * >( "logofilename2_combo" ) )
	{
		pCombo->clear();
		
		QList<QString>logoNames = readLogoNames();
		for (int i = 0; i < logoNames.size(); i++) {
			pCombo->addItem( logoNames[i], i );
		}

		pCombo->setMaxVisibleItems( pCombo->count() );
	}


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

QList<QString> GhostwriterPlugin::readLogoNames() {
	QList<QString> names;
	DIR *logos;
	struct dirent *d;

	if (m_logoDirName != NULL) {
		logos = opendir(m_logoDirName);
		if (logos != NULL) {
			while ((d = readdir(logos)) != NULL) {
				if (strcmp(d->d_name, ".") != 0 && strcmp(d->d_name, "..") != 0) {
					names << d->d_name;
				}
			}
			closedir(logos);
			free(logos);
		}
	}
	return names;
}

void GhostwriterPlugin::readConfigFile() {
	char *home;
	char *config_filename;
	char *dirname;
	dirname = (char*) malloc(1024);
#ifdef WIN32
	home = getenv("USERPROFILE");
	int length = strlen(home) + strlen("\\Ghostwriter.cfg");
	config_filename = (char*) malloc(length +1);
	strcpy(config_filename, home);
	strcat(config_filename, "\\Ghostwriter.cfg");
#else
	home = getenv("HOME");
	int length = strlen(home) + strlen("/Ghostwriter.cfg");
	config_filename = (char*) malloc(length +1);
	strcpy(config_filename, home);
	strcat(config_filename, "/Ghostwriter.cfg");
#endif

	FILE *config = fopen(config_filename, "r");
	if (config == NULL) {
#ifdef WIN32
		sprintf(dirname, "%s\\Ghostwriter", home);
#else
		sprintf(dirname, "%s/Ghostwriter", home);
#endif
	} else {
		if (fgets(dirname, 1000, config) == NULL) {
#ifdef WIN32
			sprintf(dirname, "%s\\Ghostwriter", home);
#else
			sprintf(dirname, "%s/Ghostwriter", home);
#endif
		}
		fclose(config);
		dirname = chomp(dirname);
	}
printf("1. Ghostwriter Dirname = %s\n", dirname);
	struct stat statbuf;
	if (stat(dirname, &statbuf) == -1 || !S_ISDIR(statbuf.st_mode)) {
		free(dirname);
		dirname = NULL;
	}
	free(config_filename);
	if (m_logoDirName != NULL) 
		free(m_logoDirName);
	m_logoDirName = dirname;
printf("2. Ghostwriter Dirname = %s\n", dirname);
	//return dirname;
}

char* GhostwriterPlugin::chomp(char* string) {
	int i = strlen(string) -1;
	while (i > 0 && string[i] == '\n') {
		string[i] = '\0';
	}
	return string;
}
