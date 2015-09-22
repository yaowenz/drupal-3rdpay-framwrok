<?php
function payment_create($user_id, $out_trade_no, $amount, $platform, $type, $detail, $paid_time, $currency = 'CNY') {
	// TODO 查找是否有相同的支付记录
	$payment = PaymentEntity::create($user_id, $out_trade_no, $amount, $platform, $type, $detail, $paid_time, $currency = 'CNY');
	return payment_entity()->save($payment);
}

function payment_exists($platform, $out_trade_no) {
	$result = db_select('payment','p')->fields('p')
		->condition('platform', $platform)
		->condition('out_trade_no', $out_trade_no)
		->range(0, 1)
		->execute()
		->fetchAll();
	return $result;
}