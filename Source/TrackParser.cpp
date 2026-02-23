#include "TrackParser.h"

juce::String TrackParser::serialize(std::vector<Track> tracks)
{
    juce::Array<juce::var> arr;
    for (const auto& track : tracks)
        arr.add(toVar(track));
    return juce::JSON::toString(juce::var(arr));
}

std::vector<Track> TrackParser::deserialize(juce::String json)
{
    std::vector<Track> tracks;
    juce::var parsed = juce::JSON::parse(json);
    if (auto* arr = parsed.getArray())
    {
        for (auto& v : *arr)
            tracks.push_back(fromVar(v));
    }
    return tracks;
}

juce::var TrackParser::toVar(Track track)
{
    auto* obj = new juce::DynamicObject();
    obj->setProperty("name", track.name);
    obj->setProperty("url", track.url);
    obj->setProperty("duration", track.duration);
    return juce::var(obj);
}

Track TrackParser::fromVar(juce::var& v)
{
    return Track(
        v["name"],
        v["url"],
        v["duration"]
    );
}
