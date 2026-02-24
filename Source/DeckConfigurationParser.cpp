#include "DeckConfigurationParser.h"

juce::String DeckConfigurationParser::serialize(std::vector<DeckConfiguration> configs)
{
    juce::Array<juce::var> arr;
    for (const auto& config : configs)
        arr.add(toVar(config));
    return juce::JSON::toString(juce::var(arr));
}

std::vector<DeckConfiguration> DeckConfigurationParser::deserialize(juce::String json)
{
    std::vector<DeckConfiguration> configs;
    juce::var parsed = juce::JSON::parse(json);
    auto* arr = parsed.getArray();
    if (arr)
    {
        for (auto& v : *arr)
            configs.push_back(fromVar(v));
    }
    return configs;
}

juce::var DeckConfigurationParser::toVar(DeckConfiguration config)
{
    auto* obj = new juce::DynamicObject();
    obj->setProperty("name", config.name);
    obj->setProperty("trackUrl", config.trackUrl);
    obj->setProperty("speed", config.speed);
    obj->setProperty("volume", config.volume);
    obj->setProperty("lowEq",  config.lowEq);
    obj->setProperty("midEq",  config.midEq);
    obj->setProperty("highEq", config.highEq);

    juce::Array<juce::var> hotCuesArray;
    for (const auto& hotCue : config.hotCues)
        hotCuesArray.add(toVar(hotCue));

    obj->setProperty("hotCues", hotCuesArray);
    return juce::var(obj);
}

DeckConfiguration DeckConfigurationParser::fromVar(const juce::var& v)
{
    std::vector<HotCueButtonInfo> hotCues;
    if (auto* arr = v["hotCues"].getArray())
    {
        for (auto& hotCueVar : *arr)
            hotCues.push_back(fromVarToHotCue(hotCueVar));
    }
    return DeckConfiguration(
        v["name"],
        v["trackUrl"],
        v["speed"],
        v["volume"],
        v["lowEq"],
        v["midEq"],
        v["highEq"],
        hotCues
    );
}

juce::var DeckConfigurationParser::toVar(HotCueButtonInfo hotCue)
{
    auto* obj = new juce::DynamicObject();
    obj->setProperty("order", hotCue.order);
    obj->setProperty("positionInSeconds", hotCue.positionInSeconds);
    return juce::var(obj);
}

HotCueButtonInfo DeckConfigurationParser::fromVarToHotCue(const juce::var& v)
{
    return HotCueButtonInfo(
        (int) v["order"],
        (double) v["positionInSeconds"]
    );
}
