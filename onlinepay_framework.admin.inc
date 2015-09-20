<?php
function payment_admin_content() {
	$content = array();
	
	// Load all of our entities.	
	$query = db_select('payment', 'p')->extend('PagerDefault')->extend('TableSort');
	$entities = $query->fields('p')
		->limit(50)
		->orderBy('p.id', 'DESC')
		//->orderByHeader($header)
		->execute()
		->fetchAll();
		
	if (!empty($entities)) {
		foreach ($entities as $entity) {
			// Create tabular rows for our entities.	
			$rows[] = array(
				'data' => array(
					'id' => l($entity->id, entity_uri('payment', $entity)['path']),
					'uid' => $entity->uid,
					'platform' => $entity->platform,
					'amount' => $entity->currency . " " . sprintf("%.2f", ($entity->amount / 100)),
					'type' => $entity->type,
					'paid_at' => date('Y:m:d H:i:s', $entity->paid_at),
					'operations' => '详情',
				),
			);
		}
		// Put our entities into a themed table. See theme_table() for details.
		$content['entity_table'] = array(
			'#theme' => 'table',
			'#rows' => $rows,
			'#header' => array(t('ID'), t('用户'), '平台', '金额', '类型', '支付时间', '操作'),
		);
		
		$content['pager'] = array('#markup' => theme('pager'));
		
	}
	else {
		// There were no entities. Tell the user.
		$content[] = array(
			'#type' => 'item',
			'#markup' => t('No entities currently exist.'),
		);
	}
	return $content;
}
