using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CardEffect : MonoBehaviour {
    public GameObject[] cardObject;
    public GameObject effectCard;
    public GameObject testMesh;
    Vector2 rotateAmount;
    float speed = 100;

    // Use this for initialization
    void Start () {
        //EventCardDeleteMesh(cardObject[0], testMesh.transform);
        //EventCardInMesh(cardObject[0], testMesh.transform);
    }

    public void EventCardDeleteMesh(GameObject cardObj, Transform transform)
    {
        Vector3 firstPosition = new Vector3(testMesh.transform.position.x, testMesh.transform.position.y - 30000, testMesh.transform.position.z);
        Vector3 secondPosition = testMesh.transform.position;

        Quaternion firstQuaternion = new Quaternion(transform.rotation.x, transform.rotation.y, transform.rotation.z, transform.rotation.w);

        effectCard = Instantiate(cardObj, firstPosition, firstQuaternion);

        StartCoroutine(Camera.main.GetComponent<CameraController>().movePositionEffect(testMesh.transform.position));
        StartCoroutine(MoveUpCard(effectCard, firstPosition, secondPosition));
    }

    public void EventCardInMesh(GameObject cardObj, Transform transform)
    {
        Vector3 firstPosition = new Vector3(testMesh.transform.position.x, testMesh.transform.position.y - 30000, testMesh.transform.position.z);
        Vector3 secondPosition = testMesh.transform.position;

        Quaternion firstQuaternion = new Quaternion(transform.rotation.x, transform.rotation.y, transform.rotation.z, transform.rotation.w);

        effectCard = Instantiate(cardObj, firstPosition, firstQuaternion);

        StartCoroutine(Camera.main.GetComponent<CameraController>().movePositionEffect(testMesh.transform.position));
        StartCoroutine(MoveUpNoRotate(effectCard, firstPosition, secondPosition));
    }

    public IEnumerator MoveUpCard(GameObject cardObj, Vector3 start, Vector3 end)
    {
        while (effectCard.transform.position.y < end.y - 5)
        {
            effectCard.transform.position = Vector3.Lerp(effectCard.transform.position, end, Time.deltaTime);
            yield return null;
        }

        StartCoroutine(MoveRotateCard(effectCard));
    }

    public IEnumerator MoveRotateCard(GameObject cardObj)
    {
        Vector3 minScale = new Vector3(8, 8, 8);
        float amount = 0;
        while (amount < 360.0f)
        {
            Vector3 rotate = Vector3.Cross((testMesh.transform.position).normalized, testMesh.transform.right).normalized;
            cardObj.transform.RotateAround(gameObject.transform.position, rotate * 120, speed * Time.deltaTime);

            cardObj.transform.localScale = Vector3.Lerp(effectCard.transform.localScale, minScale, Time.deltaTime);

            amount += speed * Time.deltaTime;
            yield return null;
        }
        StartCoroutine(MoveInMesh(effectCard, 1));
    }

    public IEnumerator MoveUpNoRotate(GameObject cardObj, Vector3 start, Vector3 end)
    {
        while (effectCard.transform.position.y < end.y - 5)
        {
            effectCard.transform.position = Vector3.Lerp(effectCard.transform.position, end, Time.deltaTime);
            yield return null;
        }
        StartCoroutine(MoveInMesh(effectCard, 2));
    }

    public IEnumerator MoveInMesh(GameObject cardObj, int num)
    {
        Vector3 minScale = new Vector2(1, 1);
        while (effectCard.transform.localScale.x > minScale.x + 1)
        {
            cardObj.transform.localScale = Vector3.Lerp(effectCard.transform.localScale, minScale, Time.deltaTime);
            yield return null;
        }
        if (num == 1)
        {
            Camera.main.GetComponent<CameraShake>().ShakeOnce();
        }
        GameObject effectObj = gameObject.GetComponent<AllMeshController>().EffectObj[num];
        testMesh.GetComponent<MeshController>().EulerRotCalEffect(testMesh, effectObj, 1.01f);
        Destroy(effectCard);
    }
}