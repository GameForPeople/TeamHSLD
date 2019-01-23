using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class SoundSetting : MonoBehaviour
{
    public GameObject[] volume;
    private GameObject handlePos;
    private SoundManager soundmanager;

    private void Start()
    {
        handlePos = gameObject.GetComponent<Slider>().handleRect.transform.gameObject;
        soundmanager = GameObject.Find("SoundManager").GetComponent<SoundManager>();
    }

    public void IsVolumeAdjust()
    {
        for (int i = 0; i < volume.Length; i++)
        {
            if (handlePos.transform.position.x < volume[i].transform.position.x - 10)
            {
                volume[i].GetComponent<Image>().color = new Color(1, 1, 1, 0);
            }
            else
            {
                volume[i].GetComponent<Image>().color = new Color(1, 1, 1, 1);
            }
        }

        //mute일때 예외처리
        if (volume[0].GetComponent<Image>().color.a == 0)
        {
            soundmanager.MuteVolume(volume[0]);
            return;
        }

        //일반적인 음성 조절
        for(int i =1; i<volume.Length;i++)
        {
            if (volume[i].GetComponent<Image>().color.a == 0)
            {
                soundmanager.SoundVolumeControl(volume[i - 1]);
                return;
            }
        }

        //마지막칸 예외처리
        soundmanager.SoundVolumeControl(volume[7]);

        //if (gameObject.transform.parent.GetChild(int.Parse(gameObject.name) + 1).GetComponent<Image>().color.a == 0)
        //{
        //    Debug.Log(gameObject.name);
        //    //soundmanager.SoundVolumeControl(gameObject);
        //}
    }
}
