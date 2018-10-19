using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class SetTurn : MonoBehaviour
{
    private float spinTime = 2;
    private float time_;
    private bool isTrigger = false;
    static public bool isPicking = false;

    public Sprite[] orderSpr;

    public void PickCard(GameObject obj)
    {
        if (isTrigger)
            return;

        isTrigger = true;
        
        //테스트용===========================================================================
        if(GameObject.Find("NetworkManager") == null)
            if(RndNum() == 0)
            {
                gameObject.GetComponent<TurnSystem>().currentTurn = TURN.MYTURN_NOTYETFLAG;
                obj.name = "preOrder";
            }
            else
            {
                gameObject.GetComponent<TurnSystem>().currentTurn = TURN.ENEMYTURN;
                obj.name = "backOrder";
            }
        //===================================================================================

        else
        {
            if (GameObject.Find("NetworkManager").GetComponent<NetworkManager>().isAttackFirst)
            {
                gameObject.GetComponent<TurnSystem>().currentTurn = TURN.MYTURN_NOTYETFLAG;
                obj.name = "preOrder";
            }


            else
            {
                gameObject.GetComponent<TurnSystem>().currentTurn = TURN.ENEMYTURN;
                obj.name = "backOrder";
            }
        }
        
        if (obj.name.Equals("preOrder"))
            StartCoroutine(SpinCard(obj, orderSpr[0]));

        else if(obj.name.Equals("backOrder"))
            StartCoroutine(SpinCard(obj, orderSpr[1]));
    }

    public int RndNum()
    {
        return Random.Range(0,2);
    }

    IEnumerator SpinCard(GameObject obj, Sprite order)
    {
        isPicking = true;
        time_ = 0;
        while (true)
        {
            time_ += Time.deltaTime;
            obj.transform.eulerAngles = Vector3.Lerp(new Vector3(0,0,0), new Vector3(0, 180, 0), time_ / spinTime);

            if(time_ > spinTime * 0.5f)
                obj.transform.GetChild(0).GetComponent<Image>().sprite = order;

            yield return new WaitForEndOfFrame();
            if (time_ > spinTime)
                break;
        }
    }
}
