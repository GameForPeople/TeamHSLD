using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.EventSystems;
using UnityEngine.UI;

public class SoundBar : EventTrigger
{
    private SoundManager soundmanager;
    //public override void OnPointerClick(PointerEventData data)
    //{
    //    soundmanager.SoundVolumeControl(gameObject);
    //}

    private void OnEnable()
    {
        soundmanager = GameObject.Find("SoundManager").GetComponent<SoundManager>();
        //Time.timeScale = 0.0f;

        if (gameObject.transform.parent.name.Contains("BGM"))
        {
            if (soundmanager.BGMSource[0].volume * 8 > int.Parse(gameObject.name) - 1)
                gameObject.GetComponent<Image>().color = new Color(1, 1, 1, 1);
            else
                gameObject.GetComponent<Image>().color = new Color(1, 1, 1, 0);
        }
        else
        {
            if (soundmanager.SFXSource.volume * 8 > int.Parse(gameObject.name) - 1)
                gameObject.GetComponent<Image>().color = new Color(1, 1, 1, 1);
            else
                gameObject.GetComponent<Image>().color = new Color(1, 1, 1, 0);
        }
    }

    private void OnDisable()
    {
        //Time.timeScale = 1.0f;
    }

}
