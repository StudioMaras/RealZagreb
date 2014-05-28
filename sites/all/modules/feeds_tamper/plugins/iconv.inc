<?php

/**
 * @file
 * Convert charset using iconv function.
 */

$plugin = array(
  'form'     => 'feeds_tamper_iconv_form',
  'callback' => 'feeds_tamper_iconv_callback',
  'name'     => 'Convert charset',
  'multi'    => 'loop',
  'category' => 'Text',
);

function feeds_tamper_iconv_form($importer, $element_key, $settings) {
  $form = array();
  $charset_options = array(
    'ISO-8859-1'    => 'ISO-8859-1',
    'ISO-8859-16'   => 'ISO-8859-16',
    'ISO-8859-2'    => 'ISO-8859-2',
    'UTF-8'         => 'UTF-8',
    'Windows-1250'  => 'Windows-1250',
    'Windows-1252'  => 'Windows-1252',
  );
  $form['charset_from'] = array(
    '#type' => 'select',
    '#title' => t('From'),
    '#description' => t('The source value charset.'),
    '#required' => TRUE,
    '#options' => $charset_options,
    '#default_value' => isset($settings['charset_from']) ? $settings['charset_from'] : 'UTF-8',
  );
  $form['charset_to'] = array(
    '#type' => 'select',
    '#title' => t('To'),
    '#description' => t('The target value charset.'),
    '#required' => TRUE,
    '#options' => $charset_options,
    '#default_value' => isset($settings['charset_to']) ? $settings['charset_to'] : 'ISO-8859-1',
  );
  return $form;
}

function feeds_tamper_iconv_callback($result, $item_key, $element_key, &$field, $settings, $source) {
  $field = iconv($settings['charset_from'], $settings['charset_to'], $field);
}

