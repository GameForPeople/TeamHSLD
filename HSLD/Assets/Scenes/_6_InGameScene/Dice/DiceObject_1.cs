using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class DiceObject_1 : MonoBehaviour
{
    private Rigidbody   myRigidBody;

    private Vector3     forceVector;

    private bool        isOnRoll;      // 지금 구르는 중인 가?
    private bool        isOnForce;     // 지금 힘의 영향을 받는 중인가?

    private  Vector3     startPosition;  // 첫 주사위 위치.
    private  Vector3     endPosition;    // 끝나고 카메라 밖으로 치움.

    private Quaternion zeroRotation;

    // Use this for initialization
    void Start()
    {
        myRigidBody = GetComponent<Rigidbody>();
        forceVector = new Vector3(-70.0f, 10.0f, 0.0f);

        startPosition = new Vector3(42.0f, 12.0f, 25.0f);
        endPosition = new Vector3(-10000.0f, -10000.0f, -10000.0f);

        zeroRotation = new Quaternion(0.0f, 0.0f, 0.0f, 1.0f);

        myRigidBody.MovePosition(endPosition);
    }

    private void FixedUpdate()
    {
        if(isOnRoll == true)
        {
            if(isOnForce == true)
            {
                myRigidBody.AddForce(forceVector /*, ForceMode.Impulse*/ );
            }
        }
    }

    public void Roll()
    {
        StartCoroutine(_RollCoroutine());
    }

    IEnumerator _RollCoroutine()
    {
        __InitDice();
        yield return new WaitForSeconds(1.5f);
        __OffForceOfDice();
        yield return new WaitForSeconds(0.5f);
        __EndDice();
    }

    private void __InitDice()
    {
        isOnRoll = true;
        isOnForce = true;

        myRigidBody.velocity = Vector3.zero;
        myRigidBody.MoveRotation(zeroRotation);
        myRigidBody.MovePosition(startPosition);
    }

    private void __OffForceOfDice()
    {
        isOnForce = false;
    }

    private void __EndDice()
    {
        isOnRoll = false;
        myRigidBody.MovePosition(endPosition);
    }
}
