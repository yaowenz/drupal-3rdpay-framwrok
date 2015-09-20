<?php
class PaymentEntity {
	
	public $id;
	public $uid;
	public $out_trade_no;
	public $amount; // 金额，1 代表 0.01
	public $platform; // 支付平台
	public $type; // 支付 OR 退款 pay OR refund
	public $detail;
	public $paid_at;
	public $created_at;	
	public $currency = 'CNY'; // default = 'CNY'
	
	public static function instanceFromDb($row) {
		
	}
	
	/**
	 * 创建一条支付记录
	 * @param unknown $user_id
	 * @param unknown $out_trade_no 
	 * @param unknown $amount 金额，1 代表 0.01
	 * @param unknown $platform 支付平台
	 * @param unknown $type 支付 OR 退款
	 * @param unknown $detail
	 * @param unknown $paid_time
	 * @param string $currency
	 * @return PaymentEntity
	 */
	public static function create($user_id, $out_trade_no, $amount, $platform, $type, $detail, $paid_time, $currency = 'CNY') {
		return (object) array(		
			'uid' => $user_id,
			'out_trade_no' => $out_trade_no,
			'platform' => $platform,
			'amount' => $amount,
			'currency' => $currency,
			'detail' => $detail,
			'type' => $type, // PAY or REFUND	
			'created_at' => 0,
			'paid_at' => $paid_time,
		);
	}
}

class PaymentController extends DrupalDefaultEntityController {
	const table_name = 'payment';
	
	/**
	 * 保存Payment记录
	 * @param PaymentEntity $entity
	 * @return unknown|boolean
	 */
	public function save($entity) {
		$transaction = db_transaction();
		try {			
			$update = false;
					
			if (!empty($entity->id)) {
				$update = true;
			}
			else {
				$entity->created_at = time();
			}		

			if (!$update) {	
				drupal_write_record(self::table_name, $entity);
				$op = 'insert';
			}
			else {
				// Save the updated artwork.
				drupal_write_record(self::table_name, $entity, 'id');				
				$op = 'update';
			}							

			//module_invoke_all('entity_' . $op, $entity, self::table_name);
		
			// Ignore slave server temporarily to give time for the saved
			// order to be propagated to the slave.
			db_ignore_slave();
			return $entity;
		}		
		catch (Exception $e) {
			$transaction->rollback();
			watchdog_exception(self::table_name, $e, NULL, WATCHDOG_ERROR);
			return FALSE;
		}
	}	
}

/**
 * Implement of callback_entity_info_uri
 * @param unknown $entity
 */
function payment_uri($entity) {
	$uri = 'payment/' . $entity->id;

	// Different Pay Platform can alter uri display
	drupal_alter('payment_uri_callback', $uri, $entity);

	return array(
		'path' => $uri,
	);
}

/**
 * Payment Entity Controller
 * @return PaymentController
 */
function payment_entity() {	
	return entity_get_controller('payment');
}

function payment_load_multiple($ids = array(), $conditions = array(), $reset = FALSE) {
	return entity_load('payment', $ids, $conditions, $reset);
}

function payment_load($id = NULL, $reset = FALSE) {
	$id = (isset($id) ? array($id) : array());	
	$payment = payment_load_multiple($id);
	return $payment ? reset($payment) : FALSE;
}