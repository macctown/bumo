/*
bumo is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

bumo is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with bumo.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef FULLNODE_MANAGER_H_
#define FULLNODE_MANAGER_H_

#include <utils/headers.h>
#include <ledger/environment.h>
#include <common/private_key.h>
#include "fullnode.h"

namespace bumo {

	typedef std::shared_ptr<FullNode> FullNodePointer;

	class FullNodeManager : public utils::Singleton<bumo::FullNodeManager> {
	private:
		std::map<std::string, FullNodePointer> fullNodeInfo_;
		std::vector<std::string> sortedFullNodes_;
		int64_t last_ledger_seq_;
		int64_t fullnode_check_timer_;
		std::string local_address_;
		PrivateKey priv_key_;
		
	public:
		FullNodeManager();
		~FullNodeManager();

		bool Initialize();
		bool Exit();

		FullNodePointer get(std::string& key);
		bool add(FullNode& fn);
		bool remove(std::string& key);

		// head of 1/1000 check tail of 1/1000
		bool isHead1In1000(const std::string& addr, std::string& peer);
		bool isTail1In1000(const std::string& addr, std::string& peer);

		// sorted by latest block hash
		bool sortFullNode(const std::string& blockHash);

		// receiver check authority of sender  
		bool verifyCheckAuthority(const std::string& checkerAddr, const std::string& beCheckedAddr);
		
		// do full node check 
		bool check();
		bool checkResponse(protocol::WsMessage &msg);
		bool OnCheck(protocol::WsMessage &msg);

		// impeach inactive or out-sync full node
		bool impeach(const std::string& impeach_addr, const std::string& reason);

		// reward the top one of sorted full nodes list with 10% of block reward and tx fee
		bool reward(std::shared_ptr<Environment> env, int64_t fullnode_reward);
	};
};
#endif