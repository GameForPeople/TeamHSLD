using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class TutorialDisplayTxt : MonoBehaviour
{
    private float time_;
    public string text;
    public Vector3 displayPos;
    public bool trigger;
    public float delayTime;
    public Vector3 startPos;
    public Vector3 endPos;
    public int sizeX;
    public Color inputColor;

    private void OnEnable()
    {
        gameObject.GetComponentInChildren<Text>().text = text;
        gameObject.transform.GetChild(1).transform.localPosition = displayPos;
        gameObject.transform.GetChild(0).GetComponent<Image>().color = inputColor;

        switch(GameObject.FindWithTag("GameManager").GetComponent<TutorialManager>().currentTutorial)
        {
            case TUTORIAL.READY_SELECTORDERCARD:
                StartCoroutine(performance1());
                break;
            case TUTORIAL.READY_SELECTCARD:
                StartCoroutine(performance2(delayTime));
                break;
            case TUTORIAL.INGAME_ROLLINGDICE:
                StartCoroutine(performance2(delayTime));
                break;
            case TUTORIAL.INGAME_SELECTTERRAINCARD:
                StartCoroutine(performance2(delayTime));
                break;
            case TUTORIAL.INGAME_SELECTFLAG:
                StartCoroutine(performance2(delayTime));
                break;
            case TUTORIAL.INGAME_SELECTTERRAINLOC:
                StartCoroutine(performance2(delayTime));
                break;
            case TUTORIAL.INGAME_SELECTLOCDONE:
                StartCoroutine(performance2(delayTime));
                break;
            case TUTORIAL.INGAME_TURNEND:
                StartCoroutine(performance2(delayTime));
                break;
        }

    }
    private void OnDisable()
    {
        //gameObject.transform.GetChild(1).GetComponent<RectTransform>().sizeDelta = new Vector2(10, 100);
    }

    IEnumerator performance1()
    {
        time_ = 0;
        while (true)
        {
            time_ += Time.deltaTime * 5f;
            gameObject.transform.GetChild(0).localPosition = Vector3.Lerp(startPos, endPos, time_);
            gameObject.transform.GetChild(0).GetComponent<RectTransform>().sizeDelta = Vector2.Lerp(new Vector2(25, 150), new Vector2(1420, 150), time_);
            gameObject.transform.GetChild(1).GetComponent<Text>().color = Color.Lerp(new Color(1, 1, 1, 0), new Color(1, 1, 1, 1), time_);
            yield return new WaitForEndOfFrame();
            if (time_ > 1.0f)
                break;
        }
        time_ = 0;
        while (true)
        {
            yield return new WaitForEndOfFrame();
            if (TutorialSetTurn.isDone)
                break;
        }
        gameObject.SetActive(false);
    }

    IEnumerator performance2(float inputTime)
    {
        time_ = 0;
        while (true)
        {
            time_ += Time.deltaTime * 5f;
            gameObject.transform.GetChild(0).localPosition = Vector3.Lerp(startPos, endPos, time_);
            gameObject.transform.GetChild(0).GetComponent<RectTransform>().sizeDelta = Vector2.Lerp(new Vector2(25, 150), new Vector2(sizeX, 150), time_);
            gameObject.transform.GetChild(1).GetComponent<Text>().color = Color.Lerp(new Color(1, 1, 1, 0), new Color(1, 1, 1, 1), time_);
            yield return new WaitForEndOfFrame();
            if (time_ > 1.0f)
                break;
        }
        time_ = 0;
        
        yield return new WaitForSeconds(inputTime);
        gameObject.SetActive(false);
    }
}
