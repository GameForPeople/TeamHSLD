using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class TutorialSetTurn : MonoBehaviour
{
    private float time_;
    private bool isTrigger = false;
    static public bool isPicking = false;

    public Sprite orderSpr;
    static public bool isDone = false;

    public void PickCard(GameObject obj)
    {
        if (isTrigger)
            return;

        isTrigger = true;

        obj.name = "preOrder";
        StartCoroutine(SpinCard(obj, orderSpr, "선공"));
    }

    public int RndNum()
    {
        return Random.Range(0, 2);
    }

    IEnumerator SpinCard(GameObject obj, Sprite order, string val)
    {
        isDone = true;
        isPicking = true;
        time_ = 0;
        while (true)
        {
            time_ += Time.deltaTime;
            obj.transform.eulerAngles = Vector3.Lerp(new Vector3(0, 0, 0), new Vector3(0, 180, 0), time_);
            if (time_ > 0.5f)
            {
                obj.transform.localScale = new Vector3(-1, 1, 1);

                GameObject.FindWithTag("GameManager").GetComponent<TutorialTurnSystem>().mainTxt.text = "";
                GameObject.FindWithTag("GameManager").GetComponent<TutorialTurnSystem>().timerTxt.transform.localPosition = new Vector3(0, 261, 0);
                GameObject.FindWithTag("GameManager").GetComponent<TutorialTurnSystem>().timerTxt.text = val;
                obj.transform.GetChild(0).GetComponent<Image>().sprite = order;
            }

            yield return new WaitForEndOfFrame();
            if (time_ > 1f)
                break;
        }
    }
}
