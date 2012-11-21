#pragma once

#include "B5Plugin.h"
#include <QString>
#include <QObject>

// Define the settings strings - these must match the prefixes on the controls
// for the option system to work properly
#define GHOSTWRITER_ENABLED	11 //"gwenabled"	// Boolean - is the plug-in enabled?

#define GHOSTWRITER_HEIGHT			20
#define GHOSTWRITER_LUCENT			21
#define GHOSTWRITER_LOGOFILENAME	22
#define GHOSTWRITER_LOGOSIZE		23
#define GHOSTWRITER_LOGOORIENTATION	24
#define GHOSTWRITER_LOGOTRANS		25
#define GHOSTWRITER_FIT				26
// Logo2 Tab
#define GHOSTWRITER_LOGO2ENABLED	27
#define GHOSTWRITER_LOGOFILENAME2	28
#define GHOSTWRITER_LOGOSIZE2		29
#define GHOSTWRITER_XPOS2			30
#define GHOSTWRITER_YPOS2			31
#define GHOSTWRITER_LOGOTRANS2		32
// Config Tab
#define GHOSTWRITER_LOGODIR			33

#define GHOSTWRITER_PLUGINNAME		"Ghostwriter"
#define GHOSTWRITER_PLUGINGROUP		"ghostwr"// The  plugin group/owner name  for all of yout plug-in's settings keep it short but unique
#define GHOSTWRITER_GROUP			"Ghostwriter Plugin"		// The group for all of this plug-in's settings
#define GHOSTWRITER_FILTER			"Ghostwriter_Filter"

#define GHOSTWRITER_ABOUT_BNNAME	"aboutButton"
#define GHOSTWRITER_SAVE_BNNAME		"saveButton"
#define GHOSTWRITER_REFRESH_BNNAME	"refreshButton"


class GhostwriterPlugin : public QObject, public B5Plugin
{
	Q_OBJECT

public:
	GhostwriterPlugin() { ; }
	virtual int priority() { return 100; }
	virtual QString name() { return QString(GHOSTWRITER_PLUGINNAME); }
	virtual QString group() { return QString(GHOSTWRITER_PLUGINGROUP); }
	virtual bool init(PluginHub *hub, int id, int groupId, const QString &bundlePath);
	virtual bool registerFilters();
	virtual bool registerOptions();
	virtual bool finish();
	virtual PluginDependency *createDependency(const QString &name);
    virtual QList<QString> toolFiles();
    virtual QList<QWidget*> toolWidgets();
    virtual void toolWidgetCreated(QWidget *uiWidget);

private slots:
	void showAbout() const;
	void fitIntoCrop() const;
	void save() const;

private:
	PluginHub *m_hub;
	int m_id;
	int m_groupId;
	QWidget *	m_pWindow;
	char* m_logoDirName;

	QList<QString> readLogoNames();
	void readConfigFile();
	char* chomp(char* string);
};
