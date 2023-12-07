# Configuration file for the Sphinx documentation builder.
#
# For the full list of built-in configuration values, see the documentation:
# https://www.sphinx-doc.org/en/master/usage/configuration.html

# -- Project information -----------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#project-information

project = 'C++evy'
copyright = '2023, Axel Denis, Auguste Frater, Julian Scott, Ludovic De-chavagnac, Arthur Aillet'
author = 'Axel Denis, Auguste Frater, Julian Scott, Ludovic De-chavagnac, Arthur Aillet'
release = 'v0.1'

# -- General configuration ---------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#general-configuration

extensions = []

templates_path = ['_templates']
exclude_patterns = []



# -- Options for HTML output -------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#options-for-html-output

html_theme = 'sphinx_rtd_theme'
html_static_path = ['_static']


extensions = ['breathe', 'myst_parser']
breathe_projects = {'cevy': '../../doxygen/xml'}
breathe_default_project = 'cevy'
