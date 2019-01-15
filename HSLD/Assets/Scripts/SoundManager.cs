using System.Collections;
using System.Collections.Generic;
using UnityEngine;

/*
 SFX Usage
 InGameSceneManager - SoundManager - Clips보면 모든 clip이 이 안에 들어가있는데 ,
 Index번호를 ref로 날려주면 된다.

    ex)             SoundManager.instance_.SFXPlay(SoundManager.instance_.clips[10], 1.0f);
    이상끝.
     */



//동시에 BGM 두개 나오는경우에대해서는 생각해봐야할듯.
public class SoundManager : MonoBehaviour
{
    public AudioSource[] BGMSource = new AudioSource[2];
    public AudioSource SFXSource;

    public AudioClip[] clips;
    static public SoundManager instance_;

    private float time_;

    //싱글턴
    private void Awake()
    {
        if (instance_ == null)
            instance_ = this;
        else
            Destroy(gameObject);
    }

    //효과음을 한번 출력할때 사용. - Ref(플레이할 클립, 출력할 사운드 크기)
    public void SFXPlay(AudioClip clip, float volume)
    {
        SFXSource.volume = volume;
        SFXSource.PlayOneShot(clip);
    }

    public void BGMChange(AudioClip clip)
    {
        if(clip.name.Contains("LOGIN"))
        {
            BGMSource[0].clip = clip;
            BGMSource[1].clip = clips[13];
            BGMSource[0].Play();
            BGMSource[1].Play();
        }
        else if(clip.name.Contains("INGAME"))
        {
            BGMSource[0].clip = clip;
            BGMSource[0].Play();
            BGMSource[1].Stop();
        }
    }
}
