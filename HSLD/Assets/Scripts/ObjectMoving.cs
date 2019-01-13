﻿using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public enum MOVINGOBJECT
{
    CHICKEN,
    SNAKE,
    PENGUIN,
    WHALE,
    CLOUD,
}

public class ObjectMoving : MonoBehaviour
{
    public MOVINGOBJECT thisObject;
    private GameObject[] ableToGoObj = new GameObject[3];
    public int startingIndex;
    private int randomVal;
    private float time_;

    private GameObject tmpObj;

    private void Start()
    {
        StartCoroutine(Moving(GameObject.Find(startingIndex.ToString())));
    }

    void AlbeToGo(GameObject obj)
    {
        int j = 0;
        for(int i =0; i< obj.GetComponent<MeshController>().JointMesh.Length;i++)
        {
            if (obj.GetComponent<MeshController>().terrainstate.Equals(obj.GetComponent<MeshController>().JointMesh[i].GetComponent<MeshController>().terrainstate))
            {
                ableToGoObj[j] = obj.GetComponent<MeshController>().JointMesh[i];
                j += 1;
            }
        }
    }

    IEnumerator Moving(GameObject startingPoint)
    {
        AlbeToGo(startingPoint);
        randomVal = Random.Range(0, 3);

        if (ableToGoObj[randomVal] == null)
            StartCoroutine(Moving(startingPoint));
        else
        {
            switch(thisObject)
            {
                case MOVINGOBJECT.CHICKEN:
                    gameObject.transform.LookAt(ableToGoObj[randomVal].transform.position * 1.05f, Vector3.up);
                    //gameObject.transform.GetChild(0).LookAt(ableToGoObj[randomVal].transform.position * 1.05f, Vector3.back);
                    time_ = 0;
                    while (true)
                    {
                        time_ += Time.deltaTime * 0.5f;
                        gameObject.transform.position = Vector3.Lerp(startingPoint.transform.position * 1.05f, ableToGoObj[randomVal].transform.position * 1.05f, time_);
                        yield return new WaitForEndOfFrame();
                        if (time_ > 0.95f)
                            break;
                    }

                    StartCoroutine(Moving(ableToGoObj[randomVal]));

                    break;
                case MOVINGOBJECT.CLOUD:
                    gameObject.transform.LookAt(ableToGoObj[randomVal].transform.position * 1.05f, Vector3.up);

                    time_ = 0;
                    while (true)
                    {
                        time_ += Time.deltaTime * 0.1f;
                        gameObject.transform.position = Vector3.Lerp(startingPoint.transform.position * 1.05f, ableToGoObj[randomVal].transform.position * 1.05f, time_);
                        yield return new WaitForEndOfFrame();
                        if (time_ > 0.95f)
                            break;
                    }

                    StartCoroutine(Moving(ableToGoObj[randomVal]));
                    break;
                case MOVINGOBJECT.PENGUIN:
                    gameObject.transform.LookAt(ableToGoObj[randomVal].transform.position * 1.05f, Vector3.up);

                    time_ = 0;
                    while (true)
                    {
                        time_ += Time.deltaTime * 0.5f;
                        gameObject.transform.position = Vector3.Lerp(startingPoint.transform.position * 1.05f, ableToGoObj[randomVal].transform.position * 1.05f, time_);
                        yield return new WaitForEndOfFrame();
                        if (time_ > 0.95f)
                            break;
                    }

                    StartCoroutine(Moving(ableToGoObj[randomVal]));
                    break;
                case MOVINGOBJECT.SNAKE:
                    gameObject.transform.LookAt(ableToGoObj[randomVal].transform.position * 1.02f, Vector3.up);
                    //gameObject.transform.GetChild(0).LookAt(ableToGoObj[randomVal].transform.position * 1.02f, Vector3.back);
                    //tmpObj = Instantiate(Resources.Load("Prefabs/MovingObj/Pos")) as GameObject;
                    //tmpObj.transform.position = ableToGoObj[randomVal].transform.position * 1.05f;

                    time_ = 0;
                    while (true)
                    {
                        time_ += Time.deltaTime;
                        gameObject.transform.position = Vector3.Lerp(startingPoint.transform.position * 1.02f, ableToGoObj[randomVal].transform.position * 1.02f, time_);
                        yield return new WaitForEndOfFrame();
                        if (time_ > 0.98f)
                            break;
                    }

                    yield return new WaitForSeconds(1f);
                    gameObject.transform.GetChild(0).transform.GetChild(0).transform.gameObject.SetActive(true);
                    time_ = 0;
                    while (true)
                    {
                        gameObject.transform.GetChild(0).transform.GetChild(0).transform.gameObject.transform.localPosition = Vector3.Lerp(new Vector3(0, 0, -0.4f), new Vector3(0, 0, 0.1f), time_);
                        time_ += Time.deltaTime;
                        yield return new WaitForEndOfFrame();
                        if (time_ > 1f)
                            break;
                    }
                    yield return new WaitForSeconds(2f);

                    time_ = 0;
                    while (true)
                    {
                        gameObject.transform.GetChild(0).transform.GetChild(0).transform.gameObject.transform.localPosition = Vector3.Lerp(new Vector3(0, 0, 0.1f), new Vector3(0, 0, -0.4f), time_);
                        time_ += Time.deltaTime;
                        yield return new WaitForEndOfFrame();
                        if (time_ > 1f)
                            break;
                    }
                    gameObject.transform.GetChild(0).transform.GetChild(0).transform.gameObject.SetActive(false);
                    yield return new WaitForSeconds(1f);

                    StartCoroutine(Moving(ableToGoObj[randomVal]));
                    break;
                case MOVINGOBJECT.WHALE:
                    gameObject.transform.LookAt(ableToGoObj[randomVal].transform.position * 1.05f, Vector3.up);

                    time_ = 0;
                    while (true)
                    {
                        time_ += Time.deltaTime * 0.2f;
                        gameObject.transform.position = Vector3.Lerp(startingPoint.transform.position * 1.05f, ableToGoObj[randomVal].transform.position * 1.05f, time_);
                        yield return new WaitForEndOfFrame();
                        if (time_ > 0.95f)
                            break;
                    }

                    StartCoroutine(Moving(ableToGoObj[randomVal]));
                    break;
            }

           // gameObject.transform.LookAt(ableToGoObj[randomVal].transform.position * 1.05f,Vector3.up);
            //tmpObj = Instantiate(Resources.Load("Prefabs/MovingObj/Pos")) as GameObject;
            //tmpObj.transform.position = ableToGoObj[randomVal].transform.position * 1.05f;
        }
        
    }
}