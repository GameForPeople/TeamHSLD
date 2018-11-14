using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class MissionManager : MonoBehaviour
{
    public MissionInfo[] missionSet;
    private bool[] isCheck;
    private int index = 0;

    public GameObject readyDisplayMainMissionObj;
    public GameObject readyDisplaySubMissionObj;
    public GameObject inGameDisplaySubMissionObj;

    //중복되지않게 메인미션 / 서브미션 부여
    private void randomValue(int min, int max, MissionSet set)
    {
        int value = Random.Range(min, max);
        if (!isCheck[value])
        {
            isCheck[value] = true;
            switch(set)
            {
                case MissionSet.MAIN:
                    readyDisplayMainMissionObj.transform.GetChild(0).GetComponent<Text>().text = missionSet[value].text;
                    break;
                case MissionSet.SUB:
                    readyDisplaySubMissionObj.transform.GetChild(index).GetComponent<Text>().text = missionSet[value].text;
                    inGameDisplaySubMissionObj.transform.GetChild(1 + index).transform.GetChild(0).GetComponent<Text>().text = missionSet[value].text;
                    index += 1;
                    break;
            }
        }
            
        else
            randomValue(min, max, set);            
    }

    private void Start()
    {
        isCheck = new bool[missionSet.Length];
        for (int i = 0; i < isCheck.Length; i++)
            isCheck[i] = false;

        randomValue(0, 5, MissionSet.MAIN);

        for (int i = 0; i < 5; i++)
            randomValue(5, 20, MissionSet.SUB);
    }
}
