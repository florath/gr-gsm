/* -*- c++ -*- */
/* @file
 * @author Andre Florath <andre@florath.net>
 * @section LICENSE
 *
 * Gr-gsm is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 *
 * Gr-gsm is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with gr-gsm; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gnuradio/io_signature.h>
#include "misc_utils/bursts_file_sink_impl.h"

namespace gr {
namespace gsm {

void bursts_file_sink_impl::bursts_process(pmt::pmt_t msg) {
  std::cerr << "Bursts_print" << std::endl;
}

bursts_file_sink::sptr bursts_file_sink::make(std::string const &filename) {
  return gnuradio::get_initial_sptr(new bursts_file_sink_impl(filename));
}

/*
 * The private constructor
 */
bursts_file_sink_impl::bursts_file_sink_impl(std::string const &filename)
    : gr::block("bursts_printer", gr::io_signature::make(0, 0, 0),
                gr::io_signature::make(0, 0, 0)) {

  message_port_register_in(pmt::mp("bursts"));
  set_msg_handler(
      pmt::mp("bursts"),
      boost::bind(&bursts_file_sink_impl::bursts_process, this, _1));
}

/*
 * Our virtual destructor.
 */
bursts_file_sink_impl::~bursts_file_sink_impl() {}
}
}
