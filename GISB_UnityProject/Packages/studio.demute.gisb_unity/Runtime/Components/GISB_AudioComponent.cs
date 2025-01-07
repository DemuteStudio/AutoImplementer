using System.Collections;
using System.Collections.Generic;
using Unity.Collections.LowLevel.Unsafe;
using UnityEngine;
using UnityEngine.Serialization;


namespace GISB.Runtime
{
    public class GISB_AudioComponent : MonoBehaviour
    {
        [FormerlySerializedAs("associatedPlayable")] public GISB_Asset associatedAsset;
        public Dictionary<string, string> activeParameters = new Dictionary<string, string>();
        
        public Dictionary<GISB_Event, GISB_EventInstance> activeEventInstances = new Dictionary<GISB_Event, GISB_EventInstance>();

        public void PlayEvent()
        {
            PlayEvent(associatedAsset.GetEvent(""));
        }
        
        public void PlayEvent(GISB_Event eventToPlay)
        {
            if(!eventToPlay) return;

            if(!activeEventInstances.ContainsKey(eventToPlay) || activeEventInstances[eventToPlay] == null)
            {
                GameObject newEventInstanceObject = new GameObject(eventToPlay.name);
                newEventInstanceObject.transform.parent = this.transform;
                GISB_EventInstance newEventInstance = newEventInstanceObject.AddComponent<GISB_EventInstance>();
                newEventInstance.eventDefinition = eventToPlay;
                activeEventInstances[eventToPlay] = newEventInstance;
            }

            activeEventInstances[eventToPlay].Play(activeParameters);
        }
        
        public void PlayEvent(string eventName)
        {
            GISB_Event eventToPlay = associatedAsset.GetEvent(eventName);
            PlayEvent(eventToPlay);
        }

        public void SetParameter(string parameterName, string parameterValue)
        {
            activeParameters[parameterName] = parameterValue;
            foreach (KeyValuePair<GISB_Event, GISB_EventInstance> activeEventInstance in activeEventInstances)
            {
                activeEventInstance.Value.UpdateParameters(activeParameters);
            }
        }
    }
}

