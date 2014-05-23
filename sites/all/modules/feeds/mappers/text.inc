<?php

/**
 * @file
 * On behalf implementation of Feeds mapping API for text.module.
 */

/**
 * Implements hook_feeds_processor_targets_alter().
 *
 * @see FeedsProcessor::getMappingTargets()
 */
function text_feeds_processor_targets_alter(&$targets, $entity_type, $bundle_name) {
  $text_types = array(
    'list_text',
    'text',
    'text_long',
    'text_with_summary',
  );
  foreach (field_info_instances($entity_type, $bundle_name) as $name => $instance) {
    $info = field_info_field($name);

    if (in_array($info['type'], $text_types)) {
      $targets[$name] = array(
        'name' => check_plain($instance['label']),
        'callback' => 'text_feeds_set_target',
        'description' => t('The @label field of the entity.', array('@label' => $instance['label'])),
        'summary_callback' => 'feeds_mapper_summary_language',
        'form_callback' => 'feeds_mapper_form_language',
      );
    }
  }
}

/**
 * Callback for mapping text fields.
 */
function text_feeds_set_target($source, $entity, $target, array $values, $mapping = array()) {
  if (isset($source->importer->processor->config['input_format'])) {
    $format = $source->importer->processor->config['input_format'];
  }

  // Set the language of the field depending on the mapping configuration.
  $langcode = isset($mapping['language']) ? $mapping['language'] : LANGUAGE_NONE;

  $field = isset($entity->$target) ? $entity->$target : array($langcode => array());

  // If it's a new language we need to add it to the field
  if (!isset($field[$langcode])) {
    $field[$langcode] = array();
  }

  // Iterate over all values.
  foreach ($values as $value) {

    if (is_object($value) && ($value instanceof FeedsElement)) {
      $value = $value->getValue();
    }

    if (is_scalar($value) && strlen($value)) {
      $value = array('value' => (string) $value);
      if (isset($format)) {
        $value['format'] = $format;
      }

      $field[$langcode][] = $value;
    }
  }

  $entity->$target = $field;
}
