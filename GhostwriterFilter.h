#pragma once

#include "PluginLayerFilter.h"
#include "PluginHub.h"


class GhostwriterFilter : public PluginLayerFilter
{
public:
	GhostwriterFilter(PluginHub *hub, int groupId) : m_hub(hub), m_groupId(groupId) { ; }
	
	virtual QString name() const { return QString("Invert5.1_Filter"); }

	virtual bool needsToRunLayer(const PluginImageSettings &options, const PluginPipeSettings &settings, PluginOptionList &layerOptions) const;
	virtual void runLayer(const PluginImageSettings &options, const PluginPipeSettings &settings, PluginTile &tile, PluginOptionList &layerOptions, int layerPos) const;
	virtual bool needsOriginalImage() { return false; };
	
	virtual bool isSlow(const PluginImageSettings &options, const PluginPipeSettings &settings) const;
	virtual bool isLargeRadius(const PluginImageSettings &options, const PluginPipeSettings &settings) const;
	virtual bool isPixelSource(const PluginImageSettings &options, const PluginPipeSettings &settings) const;
	virtual QTransform transform(const PluginImageSettings &options, const PluginPipeSettings &settings) const;
	virtual QSize size(const PluginImageSettings &options, const PluginPipeSettings &settings, const QSize &startSize) const;
	virtual QList<PluginDependency*> prerequisites(const PluginImageSettings &options, const PluginPipeSettings &settings, PluginTile &tile) const;

private:
	PluginHub *m_hub;
	int m_groupId;
};
