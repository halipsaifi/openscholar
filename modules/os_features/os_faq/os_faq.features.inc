<?php
/**
 * @file
 * os_faq.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function os_faq_ctools_plugin_api() {
  list($module, $api) = func_get_args();
  if ($module == "context" && $api == "context") {
    return array("version" => "3");
  }
  list($module, $api) = func_get_args();
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_views_api().
 */
function os_faq_views_api() {
  return array("version" => "3.0");
}

/**
 * Implements hook_node_info().
 */
function os_faq_node_info() {
  $items = array(
    'faq' => array(
      'name' => t('FAQ'),
      'base' => 'node_content',
      'description' => 'A frequently asked question with an answer.',
      'has_title' => '1',
      'title_label' => t('Question'),
      'help' => '',
    ),
  );
  return $items;
}