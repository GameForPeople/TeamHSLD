using System.Collections;
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
    private Terrain currentTerrain;

    private Vector3 dirToTarget;
    private Vector3 localUp;

    private GameObject tmpObj;
    private bool isFirstStart = true;
    private int j = 0;
    private void OnDisable()
    {
        StopAllCoroutines();
    }
    private void OnEnable()
    {
        if (isFirstStart)
            return;

        StartCoroutine(Moving(GameObject.Find(startingIndex.ToString())));
    }

    private void Start()
    {
        isFirstStart = false;
        switch (thisObject)
        {
            case MOVINGOBJECT.CHICKEN:
                currentTerrain = Terrain.MODERATION;
                break;
            case MOVINGOBJECT.CLOUD:
                currentTerrain = Terrain.MODERATION;
                break;
            case MOVINGOBJECT.PENGUIN:
                currentTerrain = Terrain.COLD;
                break;
            case MOVINGOBJECT.SNAKE:
                currentTerrain = Terrain.BARREN;
                break;
            case MOVINGOBJECT.WHALE:
                currentTerrain = Terrain.SEA;
                break;
        }

        StartCoroutine(Moving(GameObject.Find(startingIndex.ToString())));
    }

    void AlbeToGo(GameObject obj)
    {
        j = 0;
        for (int i =0; i<3;i++)
        {
            if (obj.GetComponent<MeshController>().JointMesh[i].GetComponent<MeshController>().terrainstate.Equals(currentTerrain))
            {
                ableToGoObj[j] = obj.GetComponent<MeshController>().JointMesh[i];
                j += 1;
            }
        }
        if (j == 0)
            Destroy(gameObject);
    }

    IEnumerator Moving(GameObject startingPoint)
    {
        AlbeToGo(startingPoint);
        randomVal = Random.Range(0, j);

        if (ableToGoObj[randomVal] == null)
            StartCoroutine(Moving(startingPoint));
        else
        {
            time_ = 0;
            switch (thisObject)
            {
                case MOVINGOBJECT.CHICKEN:
                    while (true)
                    {
                       if(time_ < 0.05f)
                        {
                            dirToTarget = (startingPoint.transform.position - ableToGoObj[randomVal].transform.position).normalized;
                            localUp = gameObject.transform.GetChild(0).transform.up;
                            gameObject.transform.GetChild(0).rotation = Quaternion.FromToRotation(localUp, dirToTarget) * gameObject.transform.GetChild(0).rotation;
                        }
                        gameObject.transform.position = Vector3.Lerp(startingPoint.transform.position, ableToGoObj[randomVal].transform.position, time_);
                        time_ += Time.deltaTime * 0.5f;
                        yield return new WaitForEndOfFrame();
                        if (time_ > 0.95f)
                            break;
                    }

                    StartCoroutine(Moving(ableToGoObj[randomVal]));

                    break;
                case MOVINGOBJECT.CLOUD:
                    while (true)
                    {
                        if (time_ < 0.05f)
                        {
                            dirToTarget = (startingPoint.transform.position - ableToGoObj[randomVal].transform.position).normalized;
                            localUp = gameObject.transform.GetChild(0).transform.up;
                            gameObject.transform.GetChild(0).rotation = Quaternion.FromToRotation(localUp, dirToTarget) * gameObject.transform.GetChild(0).rotation;

                        }
                        time_ += Time.deltaTime * 0.1f;
                        gameObject.transform.position = Vector3.Lerp(startingPoint.transform.position, ableToGoObj[randomVal].transform.position, time_);
                        yield return new WaitForEndOfFrame();
                        if (time_ > 0.95f)
                            break;
                    }

                    StartCoroutine(Moving(ableToGoObj[randomVal]));
                    break;
                case MOVINGOBJECT.PENGUIN:
                    while (true)
                    {
                        if (time_ < 0.05f)
                        {
                            dirToTarget = (startingPoint.transform.position - ableToGoObj[randomVal].transform.position).normalized;
                            localUp = gameObject.transform.GetChild(0).transform.up;
                            gameObject.transform.GetChild(0).rotation = Quaternion.FromToRotation(localUp, dirToTarget) * gameObject.transform.GetChild(0).rotation;

                        }
                        time_ += Time.deltaTime * 0.5f;
                        gameObject.transform.position = Vector3.Lerp(startingPoint.transform.position, ableToGoObj[randomVal].transform.position, time_);
                        yield return new WaitForEndOfFrame();
                        if (time_ > 0.95f)
                            break;
                    }

                    StartCoroutine(Moving(ableToGoObj[randomVal]));
                    break;
                case MOVINGOBJECT.SNAKE:
                   while (true)
                    {
                        if (time_ < 0.05f)
                        {
                            dirToTarget = (startingPoint.transform.position - ableToGoObj[randomVal].transform.position).normalized;
                            localUp = gameObject.transform.GetChild(0).transform.up;
                            gameObject.transform.GetChild(0).rotation = Quaternion.FromToRotation(localUp, dirToTarget) * gameObject.transform.GetChild(0).rotation;

                        }
                        time_ += Time.deltaTime;
                        gameObject.transform.position = Vector3.Lerp(startingPoint.transform.position, ableToGoObj[randomVal].transform.position, time_);
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
                    while (true)
                    {
                        if (time_ < 0.05f)
                        {
                            dirToTarget = (startingPoint.transform.position - ableToGoObj[randomVal].transform.position).normalized;
                            localUp = gameObject.transform.GetChild(0).transform.up;
                            gameObject.transform.GetChild(0).rotation = Quaternion.FromToRotation(localUp, dirToTarget) * gameObject.transform.GetChild(0).rotation;

                        }
                        time_ += Time.deltaTime * 0.2f;
                        gameObject.transform.position = Vector3.Lerp(startingPoint.transform.position, ableToGoObj[randomVal].transform.position, time_);
                        yield return new WaitForEndOfFrame();
                        if (time_ > 0.95f)
                            break;
                    }

                    StartCoroutine(Moving(ableToGoObj[randomVal]));
                    break;
            }
        }
        
    }
}
