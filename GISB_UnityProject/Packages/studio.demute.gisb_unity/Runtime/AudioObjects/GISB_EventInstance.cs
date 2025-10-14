using System;
using System.Collections;
using System.Collections.Generic;
using GISB.Runtime;
using UnityEngine;

public class GISB_EventInstance
{
    public GISB_Event eventDefinition;
    public GISB_AudioComponent ownerComponent;
    public Queue<Action> scheduledActions = new Queue<Action>();
    
    private GISB_BaseAudioPlayer rootPlayerInstance;
    
    public GISB_EventInstance(GISB_Event eventDefinition, GISB_AudioComponent ownerComponent)
    {
        this.eventDefinition = eventDefinition;
        this.ownerComponent = ownerComponent;
        rootPlayerInstance = null;
    }

    public void Play(float fadeInTime = 0.0f)
    {
        if(rootPlayerInstance == null)
        {
            rootPlayerInstance = eventDefinition.GetPlayer();
        }
        
        if(rootPlayerInstance != null)
        {
            rootPlayerInstance.Play(this, fadeInTime, 0.0f);
        }
    }
    
    public void UpdateParameters(Dictionary<string, string> activeParameters)
    {
        if(rootPlayerInstance != null)
        {
            rootPlayerInstance.UpdateParameters(activeParameters);
        }
    }

    public void UpdateParameters(Dictionary<string, float> activeParameters)
    {
        if (rootPlayerInstance != null)
        {
            rootPlayerInstance.UpdateParameters(activeParameters);
        }
    }

    public void Stop(float fadeOutTime = 0.0f)
    {
        if (rootPlayerInstance != null)
        {
            rootPlayerInstance.Stop(fadeOutTime);
        }
    }

    public void UpdateAudioThread(double dspTime)
    {
        if (rootPlayerInstance != null)
        {
            rootPlayerInstance.UpdateAudioThread(dspTime);
        }
    }
    
    public void UpdateGameThread(float deltaTime)
    {
        while(scheduledActions.Count > 0)
        {
            scheduledActions.Dequeue().Invoke();
        }
        
        if (rootPlayerInstance != null)
        {
            rootPlayerInstance.UpdateGameThread(deltaTime);
        }
    }

    public bool IsPlaying()
    {
        return rootPlayerInstance.IsPlaying();
    }
}
