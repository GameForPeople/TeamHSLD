using System.Collections;
using System.Collections.Generic;
using UnityEngine;

//동시에 BGM 두개 나오는경우에대해서는 생각해봐야할듯.
public class SoundManager : MonoBehaviour
{
    public AudioSource[] BGMSource = new AudioSource[2];
    public AudioSource SFXSource;

    public AudioClip[] clips;
    static public SoundManager instance_;

    private float time_;
    public float BGMMaximumVolume;
    public float BGMMinimumVolume;

    //싱글턴
    private void Start()
    {
        if (instance_ == null)
            instance_ = this;

        BGMSource[0].volume = BGMMinimumVolume;
        BGMSource[1].volume = BGMMaximumVolume;
    }

    //효과음을 한번 출력할때 사용. - Ref(플레이할 클립, 출력할 사운드 크기)
    public void SFXPlay(AudioClip clip, float volume)
    {
        SFXSource.volume = volume;
        SFXSource.PlayOneShot(clip);
    }

    //배경음을 교차할때 사용. - Ref(교차될 클립, 몇초에걸쳐서 교차될것인지 시간)
    //delayTime중에 다시 이함수가 들어오면 버그 발생할것으로 예상.
    public void BGMMixing(AudioClip clip, float delayTime)
    {
        StartCoroutine(BGMMixingCor(clip, delayTime));
    }

    //BGMSource[i].volume = 1이라는것은 현재 BGMSource[i]가 사용되고있는것.
    //이BGMSource[i].volume 점점줄이고 BGMSource[1 - i].volume를 점점 키워야함.
    IEnumerator BGMMixingCor(AudioClip clip, float delayTime)
    {
        time_ = 0;
        for (int i =0; i<BGMSource.Length;i++)
        {
            if(BGMSource[i].volume == BGMMaximumVolume)
            {
                BGMSource[1 - i].clip = clip;
                BGMSource[1 - i].Play();
                while (true)
                {
                    BGMSource[1 - i].volume = Mathf.Lerp(BGMMinimumVolume, BGMMaximumVolume, time_ / delayTime);
                    BGMSource[i].volume = Mathf.Lerp(BGMMaximumVolume, BGMMinimumVolume, time_ / delayTime);

                    time_ += Time.deltaTime;
                    yield return new WaitForEndOfFrame();
                    if (time_ > delayTime)
                        break;
                }
                BGMSource[1 - i].volume = BGMMaximumVolume;
                BGMSource[i].volume = BGMMinimumVolume;
                break;
            }
        }
    }
}
