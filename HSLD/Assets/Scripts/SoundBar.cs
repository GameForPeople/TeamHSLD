using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.EventSystems;

public class SoundBar : EventTrigger
{
    public override void OnPointerClick(PointerEventData data)
    {
        GameObject.Find("SoundManager").GetComponent<SoundManager>().SoundVolumeControl(gameObject);
    }

    private void OnEnable()
    {
        Time.timeScale = 0.0f;
    }

    private void OnDisable()
    {
        Time.timeScale = 1.0f;
    }

}
